// Copyright (c) 2011-2017 The Cryptonote developers
// Copyright (c) 2017-2018 The Circle Foundation & Conceal Devs
// Copyright (c) 2018-2019 Conceal Network & Conceal Devs
// Copyright (c) 2017-2020 UltraNote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <limits>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include "CryptoNote.h"
#include "CryptoNoteConfig.h"

namespace cn {

const size_t WALLET_INVALID_TRANSACTION_ID = std::numeric_limits<size_t>::max();
const size_t WALLET_INVALID_TRANSFER_ID = std::numeric_limits<size_t>::max();
const uint32_t WALLET_UNCONFIRMED_TRANSACTION_HEIGHT = std::numeric_limits<uint32_t>::max();

enum class WalletTransactionState : uint8_t {
  SUCCEEDED = 0,
  FAILED,
  CANCELLED,
  CREATED,
  DELETED
};

enum WalletEventType {
  TRANSACTION_CREATED,
  TRANSACTION_UPDATED,
  BALANCE_UNLOCKED,
  SYNC_PROGRESS_UPDATED,
  SYNC_COMPLETED,
};

struct WalletTransactionCreatedData {
  size_t transactionIndex;
};

struct Deposit
{
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

struct WalletTransactionUpdatedData {
  size_t transactionIndex;
};

struct WalletSynchronizationProgressUpdated {
  uint32_t processedBlockCount;
  uint32_t totalBlockCount;
};

struct WalletEvent {
  WalletEventType type;
  union {
    WalletTransactionCreatedData transactionCreated;
    WalletTransactionUpdatedData transactionUpdated;
    WalletSynchronizationProgressUpdated synchronizationProgressUpdated;
  };
};

struct WalletTransaction {
  WalletTransactionState state;
  uint64_t timestamp;
  uint32_t blockHeight;
  crypto::Hash hash;
  boost::optional<crypto::SecretKey> secretKey;
  int64_t totalAmount;
  uint64_t fee;
  uint64_t creationTime;
  uint64_t unlockTime;
  std::string extra;
  bool isBase;
};

enum class WalletTransferType : uint8_t {
  USUAL = 0,
  DONATION,
  CHANGE
};

struct WalletOrder {
  std::string address;
  uint64_t amount;
};

struct WalletMessage {
  std::string address;
  std::string message;
};

struct WalletTransfer {
  WalletTransferType type;
  std::string address;
  int64_t amount;
};

struct DonationSettings {
  std::string address;
  uint64_t threshold = 0;
};

struct TransactionParameters {
  std::vector<std::string> sourceAddresses;
  std::vector<WalletOrder> destinations;
  std::vector<WalletMessage> messages;
  uint64_t fee = cn::parameters::MINIMUM_FEE;
  uint64_t mixIn = 0;
  std::string extra;
  uint64_t unlockTimestamp = 0;
  DonationSettings donation;
  std::string changeDestination;
};

struct WalletTransactionWithTransfers {
  WalletTransaction transaction;
  std::vector<WalletTransfer> transfers;
};

struct TransactionsInBlockInfo {
  crypto::Hash blockHash;
  std::vector<WalletTransactionWithTransfers> transactions;
};

class IWallet {
public:
  virtual ~IWallet() {}

  virtual void initialize(const std::string& password) = 0;
  virtual void initializeWithViewKey(const crypto::SecretKey& viewSecretKey, const std::string& password) = 0;
  virtual void load(std::istream& source, const std::string& password) = 0;
  virtual void shutdown() = 0;

  virtual void changePassword(const std::string& oldPassword, const std::string& newPassword) = 0;
  virtual void save(std::ostream& destination, bool saveDetails = true, bool saveCache = true) = 0;

  virtual size_t getAddressCount() const = 0;
  virtual std::string getAddress(size_t index) const = 0;
  virtual KeyPair getAddressSpendKey(size_t index) const = 0;
  virtual KeyPair getAddressSpendKey(const std::string& address) const = 0;
  virtual KeyPair getViewKey() const = 0;
  virtual std::string createAddress() = 0;
  virtual std::string createAddress(const crypto::SecretKey& spendSecretKey) = 0;
  virtual std::string createAddress(const crypto::PublicKey& spendPublicKey) = 0;
  virtual std::vector<std::string> createAddressList(const std::vector<crypto::SecretKey>& spendSecretKeys, bool reset = true) = 0;
  virtual void deleteAddress(const std::string& address) = 0;


  virtual uint64_t getActualBalance() const = 0;
  virtual uint64_t getActualBalance(const std::string& address) const = 0;
  virtual uint64_t getPendingBalance() const = 0;
  virtual uint64_t getPendingBalance(const std::string& address) const = 0;

  virtual size_t getTransactionCount() const = 0;
  virtual WalletTransaction getTransaction(size_t transactionIndex) const = 0;
  virtual size_t getTransactionTransferCount(size_t transactionIndex) const = 0;
  virtual WalletTransfer getTransactionTransfer(size_t transactionIndex, size_t transferIndex) const = 0;

  virtual WalletTransactionWithTransfers getTransaction(const crypto::Hash& transactionHash) const = 0;
  virtual std::vector<TransactionsInBlockInfo> getTransactions(const crypto::Hash& blockHash, size_t count) const = 0;
  virtual std::vector<TransactionsInBlockInfo> getTransactions(uint32_t blockIndex, size_t count) const = 0;
  virtual std::vector<crypto::Hash> getBlockHashes(uint32_t blockIndex, size_t count) const = 0;
  virtual uint32_t getBlockCount() const  = 0;
  virtual std::vector<WalletTransactionWithTransfers> getUnconfirmedTransactions() const = 0;
  virtual std::vector<size_t> getDelayedTransactionIds() const = 0;

  virtual size_t transfer(const TransactionParameters& sendingTransaction, crypto::SecretKey &transactionSK) = 0;

  virtual size_t makeTransaction(const TransactionParameters& sendingTransaction) = 0;
  virtual void commitTransaction(size_t transactionId) = 0;
  virtual void rollbackUncommitedTransaction(size_t transactionId) = 0;

  virtual void start() = 0;
  virtual void stop() = 0;

  //blocks until an event occurred
  virtual WalletEvent getEvent() = 0;
};

}
