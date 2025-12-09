# UltraNote Infinity Core

UltraNote Infinity is a free open source privacy-protected peer-to-peer digital cash system that is completely decentralized, without the need for a central server or trusted parties. Users hold the crypto keys to their own money and transact directly with each other, with the help of a P2P network to check for double-spending.

UltraNote Infinity features a decentralized blockchain banking core, instant untraceable crypto messages that can be decrypted with recipient's private key, an encrypted IPFS file system to transfer files up to 100MB, integrated TOR VPN and Browser with DuckDuckGo search engine, and an invoice system.

## Features

### 🔐 Security & Privacy
- End-to-end encrypted transactions
- Untraceable crypto messages
- Private key encryption for message decryption
- Integrated TOR VPN support
- Decentralized P2P network architecture

### 💰 Wallet & Transactions
- Multi-currency wallet support (XUNI)
- Deposit and withdrawal functionality  
- Transaction history with blockchain verification
- Secure payment processing
- JSON RPC API for integration

### 📁 File System
- Encrypted IPFS file transfers up to 100MB
- Secure file sharing capabilities
- Decentralized file storage

### 🌐 Network
- Peer-to-peer decentralized network
- Double-spending protection
- Blockchain consensus mechanism
- Seed node infrastructure

## Technical Stack

- **Language**: C++11/14/17
- **Build System**: CMake
- **Dependencies**: Boost, Readline, RocksDB
- **Cryptography**: CryptoNote-based encryption
- **Networking**: Custom P2P protocol

## Project Structure

```
src/
├── BlockchainExplorer/    # Blockchain explorer functionality
├── Common/               # Common utilities and helpers
├── CryptoNoteCore/       # Core blockchain logic
├── CryptoNoteProtocol/   # Network protocol implementation
├── Daemon/              # Main daemon executable
├── HTTP/                # HTTP server components
├── InProcessNode/       # In-process node implementation
├── JsonRpcServer/       # JSON RPC server
├── Logging/             # Logging system
├── Miner/               # Mining functionality
├── Mnemonics/           # Mnemonic seed generation
├── NodeRpcProxy/        # Node RPC proxy
├── P2p/                 # Peer-to-peer networking
├── PaymentGate/         # Payment gateway services
├── PaymentGateService/  # Payment gateway service
├── Platform/            # Platform-specific code
├── Rpc/                 # RPC functionality
├── Serialization/       # Data serialization
├── System/              # System utilities
├── Transfers/           # Transfer management
├── UltraNoteWallet/     # Wallet implementation
└── WalletLegacy/        # Legacy wallet support
```

## Getting Started

### Prerequisites

- GCC 4.7.3 or later, or Clang 3.3 or later
- CMake 2.8.6 or later  
- Boost 1.55 or later
- GNU Readline (optional)
- RocksDB (for Apple systems)

### Installation

#### On Linux:

```bash
sudo apt-get install build-essential git cmake libboost-all-dev libreadline-dev
git clone https://github.com/xun-project/ultranotei-core.git
cd ultranotei-core
mkdir build
cd build
cmake ..
make
```

The resulting executables can be found in `build/release/src`.

#### On Windows:

1. Install [Visual Studio 2022 Community Edition](https://visualstudio.microsoft.com/downloads/)
2. Install **Desktop development with C++**
3. Install [Boost 1.72.0](https://sourceforge.net/projects/boost/files/boost-binaries/1.72.0/boost_1_72_0-msvc-14.1-64.exe/download)

```cmd
cd ultranotei-core
mkdir build
cd build
cmake -G "Visual Studio 17 2022" .. -DBOOST_ROOT=C:/local/boost_1_72_0
cmake --build . --config Release
```

#### On Apple:

```bash
brew install git cmake boost rocksdb readline
brew link --force readline
git clone https://github.com/xun-project/ultranotei-core.git
cd ultranotei-core
mkdir build
cd build
cmake ..
make
```

### Advanced Build Options

- **Parallel build**: `make -j<number of threads>`
- **Debug build**: `make build-debug`
- **Test suite**: `make test-release` or `make test-debug`
- **Disable readline**: `cmake .. -DFORCE_READLINE=FALSE`

## Daemon Services

UltraNote Infinity provides two main daemon services:

### 1. ultranoteid - Node Daemon
The main blockchain node daemon that maintains the network and processes transactions.

### 2. walletd - Payment Gateway Daemon  
Multi-user wallet service for exchanges and online services with JSON RPC API.

### 3. ultranoteiwallet - Single Wallet Daemon
Single wallet implementation with JSON RPC capabilities.

## API Integration

The system provides JSON RPC APIs for integration:

- **Node API**: Port 43000 (default)
- **Wallet API**: Port 8070 (walletd) or 8078 (ultranoteiwallet)

Key RPC methods include:
- `getStatus()` - Get node status
- `getBalance()` - Get wallet balance  
- `createAddress()` - Create new wallet address
- `sendTransaction()` - Send funds
- `getTransactions()` - Retrieve transaction history

## Configuration

### Node Configuration
Edit `/etc/XUNI/node.conf` for node daemon settings.

### Wallet Configuration  
Edit `/etc/XUNI/paymentgate.conf` for wallet daemon settings.

### Systemd Service Files
Use the provided service files in `Doc/` directory for production deployment:
- `ultranoteid.service` - Node daemon service
- `walletd.service` - Wallet daemon service

## Documentation

- [About UltraNote Infinity](Doc/about.md) - Project overview
- [Integration Guide](Doc/integration.md) - API integration documentation
- [Payment Gateway](Doc/paymentgate.md) - Wallet daemon configuration
- [Systemd Setup](Doc/systemd.md) - Production deployment guide

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Support

For support and questions:
- Check the documentation files in `Doc/` directory
- Review existing issues on GitHub
- Contact the development team

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Version Information

- **Current Version**: 2.4 (from blockchain checkpoints)
- **Block Height**: 1,341,997+ (as of latest checkpoint)
- **Currency Symbol**: XUNI
- **Max Supply**: 21,000,000 XUNI
- **Block Time**: 2 minutes
- **Ports**: P2P: 42001, RPC: 43000

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history and changes.
