# UltraNote Infinity Soft Fork Plan

## Objective
Perform a soft fork at block 1,341,997 to:
1. Move to a new P2P network with updated seed nodes
2. Roll back the blockchain to block 1,341,997
3. Maintain existing wallet balances while preventing old network connections

## Technical Requirements

### Network Changes
1. Update seed nodes in `CryptoNoteConfig.h`:
```cpp
const std::initializer_list<const char*> SEED_NODES = {
  "new.seed1.example.com:42000",
  "new.seed2.example.com:42000" 
};
```

2. Change P2P port in `CryptoNoteConfig.h`:
```cpp
const int P2P_DEFAULT_PORT = 42001; // New port
```

3. Update network version, trusted pub key and network UUID:
```cpp
// In CryptoNoteConfig.h:
const uint8_t P2P_CURRENT_VERSION = 2;
const uint8_t P2P_MINIMUM_VERSION = 2;
const char P2P_STAT_TRUSTED_PUB_KEY[] = "8f3a813a6f7a1e1e3c7b2f9d4a5c8e0f2b6d1e3a7c9e2f4d6b8a0c3e5f7a9d2b4";

// In P2pNetworks.h:
const static boost::uuids::uuid CRYPTONOTE_NETWORK = 
{ { 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x17, 0x28, 0x39, 0x4A, 0x5B, 0x6C, 0x7D, 0x8E, 0x9F, 0x01 } };
```

### Blockchain Rollback
1. Add checkpoint at block 1,341,997 in `CryptoNoteConfig.h`:
```cpp
{1341997, "ROLLBACK_BLOCK_HASH_HERE"}
```

2. Use built-in rollback functionality:
```cpp
bool rollbackBlockchainTo(1341997);
```

### Implementation Steps

1. **Preparation Phase**
   - Set up new seed nodes
   - Prepare updated binaries with new network config
   - Communicate fork schedule to community

2. **Fork Activation**
   - At block 1,341,997:
     - Stop all nodes
     - Deploy new binaries
     - Start nodes with clean database (or rolled back)

3. **Post-Fork**
   - Monitor network stability
   - Verify wallet balances
   - Update block explorers

## Verification Steps
1. Confirm new nodes only connect to new network
2. Verify blockchain height is exactly 1,341,997
3. Check transaction history integrity

## Rollback Procedure
1. Backup existing blockchain data
2. For each node:
```bash
./ultranoteid --rollback-to-height 1341997
```
3. Verify rollback completion

## Risk Mitigation
1. Prepare rollback procedure
2. Maintain old binaries for emergency fallback
3. Monitor network health post-fork
