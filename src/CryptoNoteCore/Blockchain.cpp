// Copyright (c) 2011-2017 The Cryptonote developers
// Copyright (c) 2016-2019, The Karbo developers
// Copyright (c) 2017-2018 The Circle Foundation & Conceal Devs
// Copyright (c) 2018-2019 Conceal Network & Conceal Devs
// Copyright (c) 2017-2022 UltraNote Infinity Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <algorithm>
#include <numeric>
#include <cstdio>
#include <cmath>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <boost/foreach.hpp>
#include "Common/Math.h"
#include "Common/int-util.h"
#include "Common/ShuffleGenerator.h"
#include "Common/StdInputStream.h"
#include "Common/StdOutputStream.h"
#include "Rpc/CoreRpcServerCommandsDefinitions.h"
#include "Serialization/BinarySerializationTools.h"
#include "CryptoNoteTools.h"
#include "TransactionExtra.h"
#include "CryptoNoteConfig.h"
#include "Logging/LoggerRef.h"
#include "CryptoNoteCore/BlockInfo.h"

using namespace logging;
using namespace common;
using namespace std::chrono;

namespace {

std::string appendPath(const std::string& path, const std::string& fileName) {
  std::string result = path;
  if (!result.empty()) {
    result += '/';
  }

  result += fileName;
  return result;
}

size_t getProcessMemoryUsage() {
  PROCESS_MEMORY_COUNTERS pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
    return static_cast<size_t>(pmc.WorkingSetSize);
  }
  return 0;
}

void logMemoryUsage(const std::string& operation) {
  size_t memUsage = getProcessMemoryUsage();
  logger(DEBUGGING) << operation << " - Current memory usage: " << memUsage / 1024 << "KB";
}

}

namespace std {
bool operator<(const crypto::Hash& hash1, const crypto::Hash& hash2) {
  return memcmp(&hash1, &hash2, crypto::HASH_SIZE) < 0;
}

bool operator<(const crypto::KeyImage& keyImage1, const crypto::KeyImage& keyImage2) {
  return memcmp(&keyImage1, &keyImage2, 32) < 0;
}
}

#define CURRENT_BLOCKCACHE_STORAGE_ARCHIVE_VER 3
#define CURRENT_BLOCKCHAININDICES_STORAGE_ARCHIVE_VER 1

namespace cn {
  class BlockCacheSerializer;
  class BlockchainIndicesSerializer;
}

namespace cn {

template<typename K, typename V, typename Hash>
bool serialize(google::sparse_hash_map<K, V, Hash>& value, common::StringView name, cn::ISerializer& serializer) {
  return serializeMap(value, name, serializer, [&value](size_t size) { value.resize(size); });
}

template<typename K, typename Hash>
bool serialize(google::sparse_hash_set<K, Hash>& value, common::StringView name, cn::ISerializer& serializer) {
  size_t size = value.size();
  if (!serializer.beginArray(size, name)) {
    return false;
  }

  if (serializer.type() == ISerializer::OUTPUT) {
    for (auto& key : value) {
      serializer(const_cast<K&>(key), "");
    }
  } else {
    value.resize(size);
    while (size--) {
      K key;
      serializer(key, "");
      value.insert(key);
    }
  }

  serializer.endArray();
  return true;
}

// custom serialization to speedup cache loading
bool serialize(std::vector<std::pair<Blockchain::TransactionIndex, uint16_t>>& value, common::StringView name, cn::ISerializer& s) {
  const size_t elementSize = sizeof(std::pair<Blockchain::TransactionIndex, uint16_t>);
  size_t size = value.size() * elementSize;

  if (!s.beginArray(size, name)) {
    return false;
  }

  if (s.type() == cn::ISerializer::INPUT) {
    if (size % elementSize != 0) {
      throw std::runtime_error("Invalid vector size");
    }
    value.resize(size / elementSize);
  }

  if (size) {
    s.binary(value.data(), size, "");
  }

  s.endArray();
  return true;
}

void serialize(Blockchain::TransactionIndex& value, ISerializer& s) {
  s(value.block, "block");
  s(value.transaction, "tx");
}

class BlockCacheSerializer {

public:
  BlockCacheSerializer(Blockchain& bs, const crypto::Hash lastBlockHash, ILogger& logger) :
    m_bs(bs), m_lastBlockHash(lastBlockHash), m_loaded(false), logger(logger, "BlockCacheSerializer") {
  }

  void load(const std::string& filename) {
    try {
      std::ifstream stdStream(filename, std::ios::binary);
      if (!stdStream) {
        return;
      }

      StdInputStream stream(stdStream);
      BinaryInputStreamSerializer s(stream);
      cn::serialize(*this, s);
    } catch (std::exception& e) {
      logger(WARNING) << "loading failed: " << e.what();
    }
  }

  bool save(const std::string& filename) {
    try {
      std::ofstream file(filename, std::ios::binary);
      if (!file) {
        return false;
      }

      StdOutputStream stream(file);
      BinaryOutputStreamSerializer s(stream);
      cn::serialize(*this, s);
    } catch (std::exception&) {
      return false;
    }

    return true;
  }

  void serialize(ISerializer& s) {
    auto start = std::chrono::steady_clock::now();

    uint8_t version = CURRENT_BLOCKCACHE_STORAGE_ARCHIVE_VER;
    s(version, "version");

    // ignore old versions, do rebuild
    if (version < CURRENT_BLOCKCACHE_STORAGE_ARCHIVE_VER) {
      return;
    }

    std::string operation;
    if (s.type() == ISerializer::INPUT) {
      operation = "loading ";
      crypto::Hash blockHash;
      s(blockHash, "last_block");

      if (blockHash != m_lastBlockHash) {
        return;
      }

    } else {
      operation = "- saving ";
      s(m_lastBlockHash, "last_block");
    }

    logger(INFO, GREEN) << operation << "block index";
    s(m_bs.m_blockIndex, "block_index");

    logger(INFO, GREEN) << operation << "transaction map";
    s(m_bs.m_transactionMap, "transactions");

    logger(INFO, GREEN) << operation << "spent keys";
    s(m_bs.m_spent_keys, "spent_keys");

    logger(INFO, GREEN) << operation << "outputs";
    s(m_bs.m_outputs, "outputs");

    logger(INFO, GREEN) << operation << "multi-signature outputs";
    s(m_bs.m_multisignatureOutputs, "multisig_outputs");

    logger(INFO, GREEN) << operation << "deposit index";
    s(m_bs.m_depositIndex, "deposit_index");

    auto dur = std::chrono::steady_clock::now() - start;

    logger(INFO, GREEN) << "Serialization time: " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << "ms";

    m_loaded = true;
  }

  bool loaded() const {
    return m_loaded;
  }

private:
  LoggerRef logger;
  bool m_loaded;
  Blockchain& m_bs;
  crypto::Hash m_lastBlockHash;
};

class BlockchainIndicesSerializer {

public:
  BlockchainIndicesSerializer(Blockchain& bs, const crypto::Hash lastBlockHash, ILogger& logger) :
    m_bs(bs), m_lastBlockHash(lastBlockHash), m_loaded(false), logger(logger, "BlockchainIndicesSerializer") {
  }

  void serialize(ISerializer& s) {
    uint8_t version = CURRENT_BLOCKCHAININDICES_STORAGE_ARCHIVE_VER;

    KV_MEMBER(version);

    // ignore old versions, do rebuild
    if (version != CURRENT_BLOCKCHAININDICES_STORAGE_ARCHIVE_VER) {
      return;
    }

    std::string operation;

    if (s.type() == ISerializer::INPUT) {
      operation = "loading ";

      crypto::Hash blockHash;
      s(blockHash, "blockHash");

      if (blockHash != m_lastBlockHash) {
        return;
      }

    } else {
      operation = "- saving ";
      s(m_lastBlockHash, "blockHash");
    }

    logger(INFO, GREEN) << operation << "paymentID index";
    s(m_bs.m_paymentIdIndex, "paymentIdIndex");

    logger(INFO, GREEN) << operation << "timestamp index";
    s(m_bs.m_timestampIndex, "timestampIndex");

    logger(INFO, GREEN) << operation << "generated transactions index";
    s(m_bs.m_generatedTransactionsIndex, "generatedTransactionsIndex");

    m_loaded = true;
  }

  template<class Archive> void serialize(Archive& ar, unsigned int version) {

    // ignore old versions, do rebuild
    if (version < CURRENT_BLOCKCHAININDICES_STORAGE_ARCHIVE_VER)
      return;

    std::string operation;
    if (Archive::is_loading::value) {
      operation = "loading ";
      crypto::Hash blockHash;
      ar & blockHash;

      if (blockHash != m_lastBlockHash) {
        return;
      }

    } else {
      operation = "- saving ";
      ar & m_lastBlockHash;
    }

    logger(INFO, GREEN) << operation << "paymentID index";
    ar & m_bs.m_paymentIdIndex;

    logger(INFO, GREEN) << operation << "timestamp index";
    ar & m_bs.m_timestampIndex;

    logger(INFO, GREEN) << operation << "generated transactions index";
    ar & m_bs.m_generatedTransactionsIndex;

    m_loaded = true;
  }

  bool loaded() const {
    return m_loaded;
  }

private:
  LoggerRef logger;
  bool m_loaded;
  Blockchain& m_bs;
  crypto::Hash m_lastBlockHash;
};


Blockchain::Blockchain(const Currency& currency, tx_memory_pool& tx_pool, ILogger& logger, bool blockchainIndexesEnabled) :
logger(logger, "Blockchain"),
m_currency(currency),
m_tx_pool(tx_pool),
m_current_block_cumul_sz_limit(0),
m_checkpoints(logger),
m_blockchainIndexesEnabled(blockchainIndexesEnabled),
m_upgradeDetectorV2(currency, m_blocks, BLOCK_MAJOR_VERSION_2, logger),
m_upgradeDetectorV3(currency, m_blocks, BLOCK_MAJOR_VERSION_3, logger)

{

  m_outputs.set_deleted_key(0);
  m_multisignatureOutputs.set_deleted_key(0);
  crypto::KeyImage nullImage = boost::value_initialized<decltype(nullImage)>();
  m_spent_keys.set_deleted_key(nullImage);
}

bool Blockchain::addObserver(IBlockchainStorageObserver* observer) {
  return m_observerManager.add(observer);
}

bool Blockchain::removeObserver(IBlockchainStorageObserver* observer) {
  return m_observerManager.remove(observer);
}

bool Blockchain::checkTransactionInputs(const cn::Transaction& tx, BlockInfo& maxUsedBlock) {
  BlockInfo tail;
  //not the best implementation at this time, sorry :(
  //check is ring_signature already checked ?
  if (maxUsedBlock.empty()) {
    //not checked, lets try to check
    if (!checkTransactionInputs(tx, maxUsedBlock.height, maxUsedBlock.id, &tail)) {
      return false;
    }
  } else {
    if (maxUsedBlock.height >= getCurrentBlockchainHeight()) {
      return false;
    }

    if (getBlockIdByHeight(maxUsedBlock.height) != maxUsedBlock.id) {
      //if we already failed on this height and id, skip actual ring signature check
      return false;
    }
  }

  return check_tx_outputs(tx);
}

bool Blockchain::checkTransactionInputs(const cn::Transaction& tx, BlockInfo& maxUsedBlock, BlockInfo& lastFailed) {
  BlockInfo tail;
  //not the best implementation at this time, sorry :(
  //check is ring_signature already checked ?
  if (maxUsedBlock.empty()) {
    //not checked, lets try to check
    if (!lastFailed.empty() && getCurrentBlockchainHeight() > lastFailed.height && getBlockIdByHeight(lastFailed.height) == lastFailed.id) {
      return false; //we already sure that this tx is broken for this height
    }

    if (!checkTransactionInputs(tx, maxUsedBlock.height, maxUsedBlock.id, &tail)) {
      lastFailed = tail;
      return false;
    }
  } else {
    if (maxUsedBlock.height >= getCurrentBlockchainHeight()) {
      return false;
    }

    if (getBlockIdByHeight(maxUsedBlock.height) != maxUsedBlock.id) {
      //if we already failed on this height and id, skip actual ring signature check
      return false;
    }
  }

  return check_tx_outputs(tx);
}

bool Blockchain::checkTransactionSize(size_t blobSize) {
  if (blobSize > getCurrentCumulativeBlocksizeLimit()) {
    logger(ERROR) << "transaction is too big " << blobSize << ", maximum allowed size is " << getCurrentCumulativeBlocksizeLimit();
    return false;
  }

  return true;
}

bool Blockchain::haveSpentKeyImages(const cn::Transaction& tx) {
  return this->haveTransactionKeyImagesAsSpent(tx);
}

// pre m_blockchain_lock is locked

bool Blockchain::checkTransactionSize(size_t blobSize) {
  if (blobSize > getCurrentCumulativeBlocksizeLimit()) {
    logger(ERROR) << "transaction is too big " << blobSize << ", maximum allowed size is " << getCurrentCumulativeBlocksizeLimit();
    return false;
  }

  return true;
}

bool Blockchain::haveTransaction(const crypto::Hash &id) {
  std::lock_guard<decltype(m_blockchain_lock)> lk(m_blockchain_lock);
  return m_transactionMap.find(id) != m_transactionMap.end();
}

bool Blockchain::have_tx_keyimg_as_spent(const crypto::KeyImage &key_im) {
  std::lock_guard<decltype(m_blockchain_lock)> lk(m_blockchain_lock);
  return  m_spent_keys.find(key_im) != m_spent_keys.end();
}

uint32_t Blockchain::getCurrentBlockchainHeight() {
  std::lock_guard<std::recursive_mutex> lk(m_blockchain_lock);
  return static_cast<uint32_t>(m_blocks.size());
}

bool Blockchain::init(const std::string& config_folder, bool load_existing) {
  std::lock_guard<decltype(m_blockchain_lock)> lk(m_blockchain_lock);
  auto start_time = std::chrono::high_resolution_clock::now();
  logger(INFO, BRIGHT_WHITE) << "Initializing blockchain...";
  
  if (!config_folder.empty() && !tools::create_directories_if_necessary(config_folder)) {
    logger(ERROR, BRIGHT_RED) << "Failed to create data directory: " << m_config_folder;
    return false;
  }

  m_config_folder = config_folder;

  if (!m_blocks.open(appendPath(config_folder, m_currency.blocksFileName()), 
                    appendPath(config_folder, m_currency.blockIndexesFileName()), 1024)) {
    logger(ERROR, BRIGHT_RED) << "Failed to initialize blockchain storage";
    return false;
  }

  if (load_existing && !m_blocks.empty()) {
    size_t start_height = m_blocks.size();
    logger(INFO, BRIGHT_WHITE) << "Loading blockchain from storage, height: " << start_height;
    
    BlockCacheSerializer loader(*this, get_block_hash(m_blocks.back().bl), logger.getLogger());
    std::string cache_path = appendPath(config_folder, m_currency.blocksCacheFileName());
    
    auto cache_start = std::chrono::high_resolution_clock::now();
    loader.load(cache_path);

    if (!loader.loaded()) {
      logger(WARNING, BRIGHT_YELLOW) << "No blockchain cache found or cache invalid, rebuilding...";
      auto rebuild_start = std::chrono::high_resolution_clock::now();
      rebuildCache();
      auto rebuild_end = std::chrono::high_resolution_clock::now();
      auto rebuild_time = std::chrono::duration_cast<std::chrono::milliseconds>(rebuild_end - rebuild_start);
      logger(INFO) << "Cache rebuild completed in " << rebuild_time.count() << "ms";
    } else {
      auto cache_end = std::chrono::high_resolution_clock::now();
      auto cache_time = std::chrono::duration_cast<std::chrono::milliseconds>(cache_end - cache_start);
      logger(INFO) << "Cache loaded successfully in " << cache_time.count() << "ms";
      
      // Validate cache integrity
      if (!validateBlockchainCache()) {
        logger(ERROR, BRIGHT_RED) << "Cache validation failed, rebuilding...";
        rebuildCache();
      }
    }

    if (m_blockchainIndexesEnabled) {
      logger(INFO, BRIGHT_WHITE) << "Loading blockchain indices for explorer mode";
      auto index_start = std::chrono::high_resolution_clock::now();
      
      if (!loadBlockchainIndices()) {
        logger(ERROR, BRIGHT_RED) << "Failed to load blockchain indices";
        return false;
      }
      
      auto index_end = std::chrono::high_resolution_clock::now();
      auto index_time = std::chrono::duration_cast<std::chrono::milliseconds>(index_end - index_start);
      logger(INFO, BRIGHT_WHITE) << "Successfully loaded blockchain indices in " << index_time.count() << "ms";
    }

    size_t current_height = m_blocks.size();
    if (current_height > start_height) {
      logger(INFO) << "Loaded " << (current_height - start_height) << " new blocks";
    }
  } else {
    logger(INFO, BRIGHT_WHITE) << "No existing blockchain found, starting fresh";
    m_blocks.clear();
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  logger(INFO, BRIGHT_WHITE) << "Blockchain initialized in " << total_time.count() << "ms. Current height: " << m_blocks.size();
  return true;
}

bool Blockchain::checkCheckpoints(uint32_t& lastValidCheckpointHeight) {
  logger(INFO) << "Starting checkpoint validation...";
  auto start_time = std::chrono::high_resolution_clock::now();
  
  try {
    if (m_checkpoints.empty()) {
      logger(INFO) << "No checkpoints configured, skipping validation";
      return true;
    }
    
    if (m_blocks.empty()) {
      logger(WARNING) << "Blockchain is empty, skipping checkpoint validation";
      return true;
    }
    
    uint32_t blockchain_height = static_cast<uint32_t>(m_blocks.size());
    lastValidCheckpointHeight = 0;
    size_t checkpoints_verified = 0;
    
    // Track performance for each checkpoint
    std::vector<std::pair<uint32_t, uint64_t>> checkpoint_times;
    checkpoint_times.reserve(m_checkpoints.getCheckpoints().size());
    
    // Verify all checkpoints in ascending order
    for (const auto& checkpoint : m_checkpoints.getCheckpoints()) {
      auto cp_start_time = std::chrono::high_resolution_clock::now();
      
      if (checkpoint.first >= blockchain_height) {
        logger(INFO) << "Checkpoint at height " << checkpoint.first 
                    << " is higher than current blockchain height " << blockchain_height 
                    << ", skipping remaining checkpoints";
        break;
      }
      
      logger(DEBUGGING) << "Validating checkpoint at height " << checkpoint.first;
      
      if (!checkCheckpoint(checkpoint.first, checkpoint.second)) {
        logger(ERROR, BRIGHT_RED) << "Checkpoint validation failed at height " << checkpoint.first;
        return false;
      }
      
      auto cp_end_time = std::chrono::high_resolution_clock::now();
      auto cp_duration = std::chrono::duration_cast<std::microseconds>(cp_end_time - cp_start_time);
      checkpoint_times.push_back({checkpoint.first, cp_duration.count()});
      
      lastValidCheckpointHeight = checkpoint.first;
      checkpoints_verified++;
      
      logger(DEBUGGING) << "Checkpoint at height " << checkpoint.first 
                       << " verified in " << cp_duration.count() << "µs";
    }
    
    // Calculate performance metrics
    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    size_t final_memory = getProcessMemoryUsage();
    size_t memory_delta = final_memory > initial_memory ? final_memory - initial_memory : 0;
    
    // Calculate average and max checkpoint validation times
    uint64_t total_cp_time = 0;
    uint64_t max_cp_time = 0;
    uint32_t slowest_cp_height = 0;
    
    for (const auto& cp_time : checkpoint_times) {
      total_cp_time += cp_time.second;
      if (cp_time.second > max_cp_time) {
        max_cp_time = cp_time.second;
        slowest_cp_height = cp_time.first;
      }
    }
    
    double avg_cp_time = checkpoint_times.empty() ? 0 : total_cp_time / static_cast<double>(checkpoint_times.size());
    
    // Log comprehensive performance summary
    logger(INFO) << "Checkpoint validation completed:"
                << "\n - Total time: " << total_duration.count() << "ms"
                << "\n - Memory delta: " << memory_delta / 1024 << "KB"
                << "\n - Checkpoints verified: " << checkpoints_verified
                << "\n - Average validation time: " << avg_cp_time << "µs"
                << "\n - Slowest checkpoint: height=" << slowest_cp_height 
                << ", time=" << max_cp_time << "µs"
                << "\n - Last valid checkpoint height: " << lastValidCheckpointHeight;
    
    return true;
  } catch (const std::exception& e) {
    logger(ERROR) << "Error during checkpoint validation: " << e.what();
    return false;
  }
}

bool Blockchain::checkCheckpoint(uint32_t height, const crypto::Hash& h) {
  try {
    Block b;
    if (!getBlockByHeight(height, b)) {
      logger(ERROR) << "Failed to get block at height " << height;
      return false;
    }

    if (b.previousBlockHash != h) {
      logger(ERROR) << "Checkpoint hash mismatch at height " << height 
                   << "\nExpected: " << h
                   << "\nFound: " << b.previousBlockHash;
      return false;
    }
    
    // Performance metrics for individual checkpoint validation
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::microseconds>(end_time - start_time);
    size_t final_memory = getProcessMemoryUsage();
    size_t memory_delta = final_memory > initial_memory ? final_memory - initial_memory : 0;
    
    logger(DEBUGGING) << "Individual checkpoint validation metrics:"
                     << "\n - Height: " << height
                     << "\n - Time: " << duration.count() << "µs"
                     << "\n - Memory delta: " << memory_delta / 1024 << "KB";

    return true;
  } catch (const std::exception& e) {
    logger(ERROR) << "Error checking checkpoint at height " << height << ": " << e.what();
    return false;
  }
}

bool Blockchain::validateTransaction(const Transaction& tx, uint32_t height, bool checkSize) {
  auto start_time = std::chrono::high_resolution_clock::now();
  size_t initial_memory = getProcessMemoryUsage();
  
  try {
    if (tx.inputs.empty()) {
      logger(ERROR) << "Empty inputs in transaction " << getObjectHash(tx);
      return false;
    }

    if (checkSize) {
      uint64_t size = getObjectBinarySize(tx);
      if (size > m_currency.maxTransactionSizeLimit()) {
        logger(ERROR) << "Transaction " << getObjectHash(tx) 
                     << " exceeds size limit: " << size 
                     << " bytes, maximum allowed: " << m_currency.maxTransactionSizeLimit();
        return false;
      }
    }

    uint64_t inputAmount = 0;
    uint64_t outputAmount = 0;
    
    std::unordered_set<crypto::KeyImage> keyImages;
    for (const auto& input : tx.inputs) {
      uint64_t amount = 0;
      
      if (input.type() == typeid(KeyInput)) {
        const KeyInput& in = boost::get<KeyInput>(input);
        amount = in.amount;
        
        if (!keyImages.insert(in.keyImage).second) {
          logger(ERROR) << "Transaction " << getObjectHash(tx) 
                       << " has duplicate key image " << in.keyImage;
          return false;
        }
      } else if (input.type() == typeid(MultisignatureInput)) {
        const MultisignatureInput& in = boost::get<MultisignatureInput>(input);
        amount = in.amount;
      }
      
      if (amount == 0) {
        logger(ERROR) << "Zero input amount in transaction " << getObjectHash(tx);
        return false;
      }
      
      if (inputAmount + amount < inputAmount) {
        logger(ERROR) << "Input amount overflow in transaction " << getObjectHash(tx);
        return false;
      }
      inputAmount += amount;
    }
    
    for (const auto& output : tx.outputs) {
      if (output.amount == 0) {
        logger(ERROR) << "Zero output amount in transaction " << getObjectHash(tx);
        return false;
      }
      
      if (outputAmount + output.amount < outputAmount) {
        logger(ERROR) << "Output amount overflow in transaction " << getObjectHash(tx);
        return false;
      }
      outputAmount += output.amount;
    }
    
    if (outputAmount > inputAmount) {
      logger(ERROR) << "Transaction " << getObjectHash(tx) 
                   << " creates money: inputs=" << inputAmount 
                   << ", outputs=" << outputAmount;
      return false;
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::microseconds>(end_time - start_time);
    size_t final_memory = getProcessMemoryUsage();
    size_t memory_delta = final_memory > initial_memory ? final_memory - initial_memory : 0;
    
    logger(DEBUGGING) << "Transaction " << getObjectHash(tx) 
                     << " validated in " << duration.count() << "µs"
                     << ", Memory delta: " << memory_delta / 1024 << "KB";
    
    return true;
  } catch (const std::exception& e) {
    logger(ERROR) << "Error validating transaction " << getObjectHash(tx) 
                 << ": " << e.what();
    return false;
  }
}

bool Blockchain::haveSpentKeyImages(const cn::Transaction& tx) {
  return this->haveTransactionKeyImagesAsSpent(tx);
}

// pre m_blockchain_lock is locked

bool Blockchain::checkTransactionSize(size_t blobSize) {
  if (blobSize > getCurrentCumulativeBlocksizeLimit()) {
    logger(ERROR) << "transaction is too big " << blobSize << ", maximum allowed size is " << getCurrentCumulativeBlocksizeLimit();
    return false;
  }

  return true;
}

bool Blockchain::haveTransaction(const crypto::Hash &id) {
  std::lock_guard<decltype(m_blockchain_lock)> lk(m_blockchain_lock);
  return m_transactionMap.find(id) != m_transactionMap.end();
}

bool Blockchain::have_tx_keyimg_as_spent(const crypto::KeyImage &key_im) {
  std::lock_guard<decltype(m_blockchain_lock)> lk(m_blockchain_lock);
  return  m_spent_keys.find(key_im) != m_spent_keys.end();
}

uint32_t Blockchain::getCurrentBlockchainHeight() {
  std::lock_guard<std::recursive_mutex> lk(m_blockchain_lock);
  return static_cast<uint32_t>(m_blocks.size());
}

bool Blockchain::init(const std::string& config_folder, bool load_existing) {
  std::lock_guard<decltype(m_blockchain_lock)> lk(m_blockchain_lock);
  auto start_time = std::chrono::high_resolution_clock::now();
  logger(INFO, BRIGHT_WHITE) << "Initializing blockchain...";
  
  if (!config_folder.empty() && !tools::create_directories_if_necessary(config_folder)) {
    logger(ERROR, BRIGHT_RED) << "Failed to create data directory: " << m_config_folder;
    return false;
  }

  m_config_folder = config_folder;

  if (!m_blocks.open(appendPath(config_folder, m_currency.blocksFileName()), 
                    appendPath(config_folder, m_currency.blockIndexesFileName()), 1024)) {
    logger(ERROR, BRIGHT_RED) << "Failed to initialize blockchain storage";
    return false;
  }

  if (load_existing && !m_blocks.empty()) {
    size_t start_height = m_blocks.size();
    logger(INFO, BRIGHT_WHITE) << "Loading blockchain from storage, height: " << start_height;
    
    BlockCacheSerializer loader(*this, get_block_hash(m_blocks.back().bl), logger.getLogger());
    std::string cache_path = appendPath(config_folder, m_currency.blocksCacheFileName());
    
    auto cache_start = std::chrono::high_resolution_clock::now();
    loader.load(cache_path);

    if (!loader.loaded()) {
      logger(WARNING, BRIGHT_YELLOW) << "No blockchain cache found or cache invalid, rebuilding...";
      auto rebuild_start = std::chrono::high_resolution_clock::now();
      rebuildCache();
      auto rebuild_end = std::chrono::high_resolution_clock::now();
      auto rebuild_time = std::chrono::duration_cast<std::chrono::milliseconds>(rebuild_end - rebuild_start);
      logger(INFO) << "Cache rebuild completed in " << rebuild_time.count() << "ms";
    } else {
      auto cache_end = std::chrono::high_resolution_clock::now();
      auto cache_time = std::chrono::duration_cast<std::chrono::milliseconds>(cache_end - cache_start);
      logger(INFO) << "Cache loaded successfully in " << cache_time.count() << "ms";
      
      // Validate cache integrity
      if (!validateBlockchainCache()) {
        logger(ERROR, BRIGHT_RED) << "Cache validation failed, rebuilding...";
        rebuildCache();
      }
    }

    if (m_blockchainIndexesEnabled) {
      logger(INFO, BRIGHT_WHITE) << "Loading blockchain indices for explorer mode";
      auto index_start = std::chrono::high_resolution_clock::now();
      
      if (!loadBlockchainIndices()) {
        logger(ERROR, BRIGHT_RED) << "Failed to load blockchain indices";
        return false;
      }
      
      auto index_end = std::chrono::high_resolution_clock::now();
      auto index_time = std::chrono::duration_cast<std::chrono::milliseconds>(index_end - index_start);
      logger(INFO, BRIGHT_WHITE) << "Successfully loaded blockchain indices in " << index_time.count() << "ms";
    }

    size_t current_height = m_blocks.size();
    if (current_height > start_height) {
      logger(INFO) << "Loaded " << (current_height - start_height) << " new blocks";
    }
  } else {
    logger(INFO, BRIGHT_WHITE) << "No existing blockchain found, starting fresh";
    m_blocks.clear();
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  logger(INFO, BRIGHT_WHITE) << "Blockchain initialized in " << total_time.count() << "ms. Current height: " << m_blocks.size();
  return true;
}

bool Blockchain::checkCheckpoints(uint32_t& lastValidCheckpointHeight) {
  logger(INFO) << "Starting checkpoint validation...";
  auto start_time = std::chrono::high_resolution_clock::now();
  
  try {
    if (m_checkpoints.empty()) {
      logger(INFO) << "No checkpoints configured, skipping validation";
      return true;
    }
    
    if (m_blocks.empty()) {
      logger(WARNING) << "Blockchain is empty, skipping checkpoint validation";
      return true;
    }
    
    uint32_t blockchain_height = static_cast<uint32_t>(m_blocks.size());
    lastValidCheckpointHeight = 0;
    size_t checkpoints_verified = 0;
    
    // Track performance for each checkpoint
    std::vector<std::pair<uint32_t, uint64_t>> checkpoint_times;
    checkpoint_times.reserve(m_checkpoints.getCheckpoints().size());
    
    // Verify all checkpoints in ascending order
    for (const auto& checkpoint : m_checkpoints.getCheckpoints()) {
      if (checkpoint.first >= blockchain_height) {
        logger(INFO) << "Checkpoint at height " << checkpoint.first 
                    << " is higher than current blockchain height " << blockchain_height 
                    << ", skipping remaining checkpoints";
        break;
      }
      
      if (!checkCheckpoint(checkpoint.first, checkpoint.second)) {
        logger(ERROR, BRIGHT_RED) << "Checkpoint failed at height " << checkpoint.first;
        return false;
      }
      
      lastValidCheckpointHeight = checkpoint.first;
      checkpoints_verified++;
      
      logger(DEBUGGING) << "Verified checkpoint at height " << checkpoint.first;
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    logger(INFO) << "Checkpoint validation completed in " << duration.count() << "ms. "
                << "Verified " << checkpoints_verified << " checkpoints. "
                << "Last valid checkpoint height: " << lastValidCheckpointHeight;
    
    return true;
  } catch (const std::exception& e) {
    logger(ERROR) << "Error during checkpoint validation: " << e.what();
    return false;
  }
}

bool Blockchain::checkCheckpoint(uint32_t height, const crypto::Hash& h) {
  try {
    Block b;
    if (!getBlockByHeight(height, b)) {
      logger(ERROR) << "Failed to get block at height " << height;
      return false;
    }

    if (b.previousBlockHash != h) {
      logger(ERROR) << "Checkpoint hash mismatch at height " << height 
                   << "\nExpected: " << h
                   << "\nFound: " << b.previousBlockHash;
      return false;
    }

    return true;
  } catch (const std::exception& e) {
    logger(ERROR) << "Error checking checkpoint at height " << height << ": " << e.what();
    return false;
  }
}

bool Blockchain::validateTransaction(const Transaction& transaction, uint32_t height, bool checkSize) {
  auto start_time = std::chrono::high_resolution_clock::now();
  size_t initial_memory = getProcessMemoryUsage();
  
  try {
    if (transaction.inputs.empty()) {
      logger(ERROR) << "Empty inputs in transaction " << getObjectHash(transaction);
      return false;
    }

    if (checkSize) {
      uint64_t size = getObjectBinarySize(transaction);
      if (size > m_currency.maxTransactionSizeLimit()) {
        logger(ERROR) << "Transaction " << getObjectHash(transaction) 
                     << " exceeds size limit: " << size 
                     << " bytes, maximum allowed: " << m_currency.maxTransactionSizeLimit();
        return false;
      }
    }

    uint64_t inputsAmount = 0;
    uint64_t outputsAmount = 0;
    
    std::unordered_set<crypto::KeyImage> keyImages;
    for (const auto& input : transaction.inputs) {
      uint64_t amount = 0;
      
      if (input.type() == typeid(KeyInput)) {
        const KeyInput& in = boost::get<KeyInput>(input);
        amount = in.amount;
        
        if (!keyImages.insert(in.keyImage).second) {
          logger(ERROR) << "Transaction " << getObjectHash(transaction) 
                       << " has duplicate key image " << in.keyImage;
          return false;
        }
      } else if (input.type() == typeid(MultisignatureInput)) {
        const MultisignatureInput& in = boost::get<MultisignatureInput>(input);
        amount = in.amount;
      }
      
      if (amount == 0) {
        logger(ERROR) << "Zero input amount in transaction " << getObjectHash(transaction);
        return false;
      }
      
      if (inputsAmount + amount < inputsAmount) {
        logger(ERROR) << "Input amount overflow in transaction " << getObjectHash(transaction);
        return false;
      }
      inputsAmount += amount;
    }
    
    for (const auto& output : transaction.outputs) {
      if (output.amount == 0) {
        logger(ERROR) << "Zero output amount in transaction " << getObjectHash(transaction);
        return false;
      }
      
      if (outputsAmount + output.amount < outputsAmount) {
        logger(ERROR) << "Output amount overflow in transaction " << getObjectHash(transaction);
        return false;
      }
      outputsAmount += output.amount;
    }
    
    if (outputsAmount > inputsAmount) {
      logger(ERROR) << "Transaction " << getObjectHash(transaction) 
                   << " creates money: inputs=" << inputsAmount 
                   << ", outputs=" << outputsAmount;
      return false;
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::microseconds>(end_time - start_time);
    size_t final_memory = getProcessMemoryUsage();
    size_t memory_delta = final_memory > initial_memory ? final_memory - initial_memory : 0;
    
    logger(DEBUGGING) << "Transaction " << getObjectHash(transaction) 
                     << " validated in " << duration.count() << "µs"
                     << ", Memory delta: " << memory_delta / 1024 << "KB";
    
    return true;
  } catch (const std::exception& e) {
    logger(ERROR) << "Error validating transaction " << getObjectHash(transaction) 
                 << ": " << e.what();
    return false;
  }
}

bool Blockchain::checkTransactionInputs(const cn::Transaction& tx, uint32_t height, crypto::Hash& blockId, BlockInfo* tail) {
  BlockInfo tailInfo;
  if (!tail) {
    tail = &tailInfo;
  }

  if (!checkTransactionInputs(tx, height, blockId)) {
    return false;
  }

  return true;
}

uint64_t Blockchain::getCurrentCumulativeBlocksizeLimit() const {
  return m_currency.maxBlockCumulativeSize(getCurrentBlockchainHeight());
}

bool Blockchain::check_tx_outputs(const Transaction& tx) const {
  for (const auto& out : tx.outputs) {
    if (out.target.type() == typeid(KeyOutput)) {
      if (!check_key(boost::get<KeyOutput>(out.target).key)) {
        return false;
      }
    } else if (out.target.type() == typeid(MultisignatureOutput)) {
      const MultisignatureOutput& multisignatureOutput = ::boost::get<MultisignatureOutput>(out.target);
      if (multisignatureOutput.requiredSignatureCount > multisignatureOutput.keys.size()) {
        return false;
      }
      for (const crypto::PublicKey& key : multisignatureOutput.keys) {
        if (!check_key(key)) {
          return false;
        }
      }
    }
  }
  return true;
}

// ... rest of the code remains the same ...