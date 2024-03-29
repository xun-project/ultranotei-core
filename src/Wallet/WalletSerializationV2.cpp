// Copyright (c) 2012-2016, The CryptoNote developers, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Bytecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bytecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Bytecoin.  If not, see <http://www.gnu.org/licenses/>.

#include "WalletSerializationV2.h"
#include "IWallet.h"
#include "CryptoNoteCore/CryptoNoteSerialization.h"
#include "Serialization/BinaryInputStreamSerializer.h"
#include "Serialization/BinaryOutputStreamSerializer.h"

using namespace common;
using namespace crypto;

namespace {

//DO NOT CHANGE IT
struct UnlockTransactionJobDtoV2 {
  uint32_t blockHeight;
  Hash transactionHash;
  crypto::PublicKey walletSpendPublicKey;
};

//DO NOT CHANGE IT
struct WalletTransactionDtoV2 {
  WalletTransactionDtoV2() {
  }

  WalletTransactionDtoV2(const cn::WalletTransaction& wallet) {
    state = wallet.state;
    timestamp = wallet.timestamp;
    blockHeight = wallet.blockHeight;
    hash = wallet.hash;
    depositCount = wallet.depositCount;
    firstDepositId = wallet.firstDepositId;
    totalAmount = wallet.totalAmount;
    fee = wallet.fee;
    creationTime = wallet.creationTime;
    unlockTime = wallet.unlockTime;
    extra = wallet.extra;
    isBase = wallet.isBase;
  }

  cn::WalletTransactionState state;
  uint64_t timestamp;
  uint32_t blockHeight;
  Hash hash;
  size_t depositCount;
  size_t firstDepositId;
  int64_t totalAmount;
  uint64_t fee;
  uint64_t creationTime;
  uint64_t unlockTime;
  std::string extra;
  bool isBase;
};

//DO NOT CHANGE IT
struct WalletTransferDtoV2 {
  WalletTransferDtoV2() {
  }

  WalletTransferDtoV2(const cn::WalletTransfer& tr) {
    address = tr.address;
    amount = tr.amount;
    type = static_cast<uint8_t>(tr.type);
  }

  std::string address;
  uint64_t amount;
  uint8_t type;
};

struct WalletDepositDtoV2 {
  WalletDepositDtoV2() {
  }

  WalletDepositDtoV2(const cn::Deposit& wd) {

    creatingTransactionId = wd.creatingTransactionId;
    spendingTransactionId = wd.spendingTransactionId;
    term = wd.term;
    amount = wd.amount;
    interest = wd.interest;
    height = wd.height;
    unlockHeight = wd.unlockHeight;
    locked = wd.locked;
    transactionHash = wd.transactionHash;
    outputInTransaction = wd.outputInTransaction;
    address = wd.address;
  }

  size_t creatingTransactionId;
  size_t spendingTransactionId;
  uint32_t term;
  uint64_t amount;
  uint64_t interest;
  uint64_t height;
  uint64_t unlockHeight;
  bool locked;
  uint32_t outputInTransaction;
  crypto::Hash transactionHash;
  std::string address;
};

void serialize(UnlockTransactionJobDtoV2& value, cn::ISerializer& serializer) {
  serializer(value.blockHeight, "blockHeight");
  serializer(value.transactionHash, "transactionHash");
  serializer(value.walletSpendPublicKey, "walletSpendPublicKey");
}

void serialize(WalletTransactionDtoV2& value, cn::ISerializer& serializer) {
  typedef std::underlying_type<cn::WalletTransactionState>::type StateType;

  StateType state = static_cast<StateType>(value.state);
  serializer(state, "state");
  value.state = static_cast<cn::WalletTransactionState>(state);

  serializer(value.timestamp, "timestamp");
  cn::serializeBlockHeight(serializer, value.blockHeight, "blockHeight");
  serializer(value.hash, "hash");
  serializer(value.totalAmount, "totalAmount");
  serializer(value.fee, "fee");
  serializer(value.depositCount, "depositCount");
  serializer(value.firstDepositId, "firstDepositId");
  serializer(value.creationTime, "creationTime");
  serializer(value.unlockTime, "unlockTime");
  serializer(value.extra, "extra");
  serializer(value.isBase, "isBase");
}

void serialize(WalletTransferDtoV2& value, cn::ISerializer& serializer) {
  serializer(value.address, "address");
  serializer(value.amount, "amount");
  serializer(value.type, "type");
}

void serialize(WalletDepositDtoV2& value, cn::ISerializer& serializer) {
  serializer(value.height, "height");
  serializer(value.transactionHash, "transactionHash");
  serializer(value.outputInTransaction, "outputInTransaction");
  serializer(value.creatingTransactionId, "creatingTransactionId");
  serializer(value.spendingTransactionId, "spendingTransactionId");
  serializer(value.amount, "amount");
  serializer(value.term, "term");
  serializer(value.interest, "interest");
  serializer(value.unlockHeight, "unlockHeight");
  serializer(value.locked, "locked");
  serializer(value.address, "address");
}

}

namespace cn {

WalletSerializerV2::WalletSerializerV2(
  ITransfersObserver& transfersObserver,
  crypto::PublicKey& viewPublicKey,
  crypto::SecretKey& viewSecretKey,
  uint64_t& actualBalance,
  uint64_t& pendingBalance,
    uint64_t& lockedDepositBalance,
    uint64_t& unlockedDepositBalance,  
  WalletsContainer& walletsContainer,
  TransfersSyncronizer& synchronizer,
  UnlockTransactionJobs& unlockTransactions,
  WalletTransactions& transactions,
  WalletTransfers& transfers,
  WalletDeposits& deposits,
  UncommitedTransactions& uncommitedTransactions,
  std::string& extra,
  uint32_t transactionSoftLockTime
) :
  m_transfersObserver(transfersObserver),
  m_actualBalance(actualBalance),
  m_pendingBalance(pendingBalance),
  m_lockedDepositBalance(lockedDepositBalance),
  m_unlockedDepositBalance(unlockedDepositBalance),
  m_walletsContainer(walletsContainer),
  m_synchronizer(synchronizer),
  m_unlockTransactions(unlockTransactions),
  m_transactions(transactions),
  m_transfers(transfers),
  m_deposits(deposits),
  m_uncommitedTransactions(uncommitedTransactions),
  m_extra(extra),
  m_transactionSoftLockTime(transactionSoftLockTime)
{
}

void WalletSerializerV2::load(common::IInputStream& source, uint8_t version) {
  cn::BinaryInputStreamSerializer s(source);

  uint8_t saveLevelValue;
  s(saveLevelValue, "saveLevel");
  WalletSaveLevel saveLevel = static_cast<WalletSaveLevel>(saveLevelValue);

  loadKeyListAndBanalces(s, saveLevel == WalletSaveLevel::SAVE_ALL);

  if (saveLevel == WalletSaveLevel::SAVE_KEYS_AND_TRANSACTIONS || saveLevel == WalletSaveLevel::SAVE_ALL) {
    loadTransactions(s);
    loadTransfers(s);
    loadDeposits(s);
  }

  if (saveLevel == WalletSaveLevel::SAVE_ALL) {
    loadTransfersSynchronizer(s);
    loadUnlockTransactionsJobs(s);
    s(m_uncommitedTransactions, "uncommitedTransactions");
  }

  s(m_extra, "extra");
}

void WalletSerializerV2::save(common::IOutputStream& destination, WalletSaveLevel saveLevel) {
  cn::BinaryOutputStreamSerializer s(destination);

  uint8_t saveLevelValue = static_cast<uint8_t>(saveLevel);
  s(saveLevelValue, "saveLevel");

  saveKeyListAndBanalces(s, saveLevel == WalletSaveLevel::SAVE_ALL);

  if (saveLevel == WalletSaveLevel::SAVE_KEYS_AND_TRANSACTIONS || saveLevel == WalletSaveLevel::SAVE_ALL) {
    saveTransactions(s);
    saveTransfers(s);
    saveDeposits(s);
  }

  if (saveLevel == WalletSaveLevel::SAVE_ALL) {
    saveTransfersSynchronizer(s);
    saveUnlockTransactionsJobs(s);
    s(m_uncommitedTransactions, "uncommitedTransactions");
  }

  s(m_extra, "extra");
}

std::unordered_set<crypto::PublicKey>& WalletSerializerV2::addedKeys() {
  return m_addedKeys;
}

std::unordered_set<crypto::PublicKey>& WalletSerializerV2::deletedKeys() {
  return m_deletedKeys;
}

void WalletSerializerV2::loadKeyListAndBanalces(cn::ISerializer& serializer, bool saveCache) {
  size_t walletCount;
  serializer(walletCount, "walletCount");

  m_actualBalance = 0;
  m_pendingBalance = 0;
  m_lockedDepositBalance = 0;
  m_unlockedDepositBalance = 0;
  m_deletedKeys.clear();

  std::unordered_set<crypto::PublicKey> cachedKeySet;
  auto& index = m_walletsContainer.get<KeysIndex>();
  for (size_t i = 0; i < walletCount; ++i) {
    crypto::PublicKey spendPublicKey;
    uint64_t actualBalance;
    uint64_t pendingBalance;
    uint64_t lockedDepositBalance;
    uint64_t unlockedDepositBalance;
    serializer(spendPublicKey, "spendPublicKey");

    if (saveCache) {
      serializer(actualBalance, "actualBalance");
      serializer(pendingBalance, "pendingBalance");
      serializer(lockedDepositBalance, "lockedDepositBalance");
      serializer(unlockedDepositBalance, "unlockedDepositBalance");
    }

    cachedKeySet.insert(spendPublicKey);

    auto it = index.find(spendPublicKey);
    if (it == index.end()) {
      m_deletedKeys.emplace(std::move(spendPublicKey));
    } else if (saveCache) {
      m_actualBalance += actualBalance;
      m_pendingBalance += pendingBalance;
      m_lockedDepositBalance += lockedDepositBalance;
      m_unlockedDepositBalance += unlockedDepositBalance;

      index.modify(it, [actualBalance, pendingBalance, lockedDepositBalance, unlockedDepositBalance](WalletRecord& wallet) {
        wallet.actualBalance = actualBalance;
        wallet.pendingBalance = pendingBalance;
        wallet.lockedDepositBalance = lockedDepositBalance;
        wallet.unlockedDepositBalance = unlockedDepositBalance;

      });
    }
  }

  for (auto wallet : index) {
    if (cachedKeySet.count(wallet.spendPublicKey) == 0) {
      m_addedKeys.insert(wallet.spendPublicKey);
    }
  }
}

void WalletSerializerV2::saveKeyListAndBanalces(cn::ISerializer& serializer, bool saveCache) {
  auto walletCount = m_walletsContainer.get<RandomAccessIndex>().size();
  serializer(walletCount, "walletCount");
  for (auto wallet : m_walletsContainer.get<RandomAccessIndex>()) {
    serializer(wallet.spendPublicKey, "spendPublicKey");

    if (saveCache) {
      serializer(wallet.actualBalance, "actualBalance");
      serializer(wallet.pendingBalance, "pendingBalance");
      serializer(wallet.lockedDepositBalance, "lockedDepositBalance");
      serializer(wallet.unlockedDepositBalance, "unlockedDepositBalance");
    }
  }
}

void WalletSerializerV2::loadTransactions(cn::ISerializer& serializer) {
  uint64_t count = 0;
  serializer(count, "transactionCount");

  m_transactions.get<RandomAccessIndex>().reserve(count);

  for (uint64_t i = 0; i < count; ++i) {
    WalletTransactionDtoV2 dto;
    serializer(dto, "transaction");

    WalletTransaction tx;
    tx.state = dto.state;
    tx.timestamp = dto.timestamp;
    tx.blockHeight = dto.blockHeight;
    tx.hash = dto.hash;
    tx.depositCount = dto.depositCount;
    tx.firstDepositId = dto.firstDepositId;
    tx.totalAmount = dto.totalAmount;
    tx.fee = dto.fee;
    tx.creationTime = dto.creationTime;
    tx.unlockTime = dto.unlockTime;
    tx.extra = dto.extra;
    tx.isBase = dto.isBase;

    m_transactions.get<RandomAccessIndex>().emplace_back(std::move(tx));
  }
}

void WalletSerializerV2::loadDeposits(cn::ISerializer& serializer) {
  uint64_t count = 0;
  serializer(count, "depositCount");

  m_deposits.get<RandomAccessIndex>().reserve(count);

  for (uint64_t i = 0; i < count; ++i) {
    WalletDepositDtoV2 dto;
    serializer(dto, "deposit");

    Deposit dp;
    dp.creatingTransactionId = dto.creatingTransactionId;
    dp.spendingTransactionId = dto.spendingTransactionId;
    dp.term = dto.term;
    dp.amount = dto.amount;
    dp.interest = dto.interest;
    dp.height = dto.height;
    dp.unlockHeight = dto.unlockHeight;
    dp.locked = dto.locked;
    dp.transactionHash = dto.transactionHash;
    dp.outputInTransaction = dto.outputInTransaction;
    dp.address = dto.address;

    m_deposits.get<RandomAccessIndex>().emplace_back(std::move(dp));
  }
}

void WalletSerializerV2::saveTransactions(cn::ISerializer& serializer) {
  uint64_t count = m_transactions.size();
  serializer(count, "transactionCount");

  for (const auto& tx : m_transactions) {
    WalletTransactionDtoV2 dto(tx);
    serializer(dto, "transaction");
  }
}

void WalletSerializerV2::saveDeposits(cn::ISerializer& serializer) {
  uint64_t count = m_deposits.size();
  serializer(count, "depositCount");

  for (const auto& tx : m_deposits) {
    WalletDepositDtoV2 dto(tx);
    serializer(dto, "deposit");
  }
}

void WalletSerializerV2::loadTransfers(cn::ISerializer& serializer) {
  uint64_t count = 0;
  serializer(count, "transferCount");

  m_transfers.reserve(count);

  for (uint64_t i = 0; i < count; ++i) {
    uint64_t txId = 0;
    serializer(txId, "transactionId");

    WalletTransferDtoV2 dto;
    serializer(dto, "transfer");

    WalletTransfer tr;
    tr.address = dto.address;
    tr.amount = dto.amount;
    tr.type = static_cast<WalletTransferType>(dto.type);

    m_transfers.emplace_back(std::piecewise_construct, std::forward_as_tuple(txId), std::forward_as_tuple(std::move(tr)));
  }
}

void WalletSerializerV2::saveTransfers(cn::ISerializer& serializer) {
  uint64_t count = m_transfers.size();
  serializer(count, "transferCount");

  for (const auto& kv : m_transfers) {
    uint64_t txId = kv.first;

    WalletTransferDtoV2 tr(kv.second);

    serializer(txId, "transactionId");
    serializer(tr, "transfer");
  }
}

void WalletSerializerV2::loadTransfersSynchronizer(cn::ISerializer& serializer) {
  std::string transfersSynchronizerData;
  serializer(transfersSynchronizerData, "transfersSynchronizer");

  std::stringstream stream(transfersSynchronizerData);
  m_synchronizer.load(stream);
}

void WalletSerializerV2::saveTransfersSynchronizer(cn::ISerializer& serializer) {
  std::stringstream stream;
  m_synchronizer.save(stream);
  stream.flush();

  std::string transfersSynchronizerData = stream.str();
  serializer(transfersSynchronizerData, "transfersSynchronizer");
}

void WalletSerializerV2::loadUnlockTransactionsJobs(cn::ISerializer& serializer) {
  auto& index = m_unlockTransactions.get<TransactionHashIndex>();
  auto& walletsIndex = m_walletsContainer.get<KeysIndex>();

  uint64_t jobsCount = 0;
  serializer(jobsCount, "unlockTransactionsJobsCount");

  for (uint64_t i = 0; i < jobsCount; ++i) {
    UnlockTransactionJobDtoV2 dto;
    serializer(dto, "unlockTransactionsJob");

    auto walletIt = walletsIndex.find(dto.walletSpendPublicKey);
    if (walletIt != walletsIndex.end()) {
      UnlockTransactionJob job;
      job.blockHeight = dto.blockHeight;
      job.transactionHash = dto.transactionHash;
      job.container = walletIt->container;

      index.emplace(std::move(job));
    }
  }
}

void WalletSerializerV2::saveUnlockTransactionsJobs(cn::ISerializer& serializer) {
  auto& index = m_unlockTransactions.get<TransactionHashIndex>();
  auto& wallets = m_walletsContainer.get<TransfersContainerIndex>();

  uint64_t jobsCount = index.size();
  serializer(jobsCount, "unlockTransactionsJobsCount");

  for (const auto& j : index) {
    auto containerIt = wallets.find(j.container);
    assert(containerIt != wallets.end());

    auto keyIt = m_walletsContainer.project<KeysIndex>(containerIt);
    assert(keyIt != m_walletsContainer.get<KeysIndex>().end());

    UnlockTransactionJobDtoV2 dto;
    dto.blockHeight = j.blockHeight;
    dto.transactionHash = j.transactionHash;
    dto.walletSpendPublicKey = keyIt->spendPublicKey;

    serializer(dto, "unlockTransactionsJob");
  }
}

} //namespace CryptoNote
