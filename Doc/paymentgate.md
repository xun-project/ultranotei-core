# Payment Gateway Documentation

## Overview
This document provides comprehensive documentation for the UltraNote Infinity payment gateway system.

## Configuration

### Required Parameters
```ini
container-file=            # Path to wallet container
container-password=        # Container password
```

### Optional Parameters
```ini
data-dir=/srv/ultranote    # Blockchain data directory
bind-address=0.0.0.0       # Server IP address
bind-port=8070             # RPC port
rpc-user=                  # RPC authentication user
rpc-password=              # RPC authentication password
server-root=/srv/ultranote # Working directory
log-level=                 # Log verbosity (0-4)
local=1                    # In-process node mode
daemon-address=localhost   # Remote daemon address
daemon-port=30000          # Remote daemon port
```

## RPC Methods

### getStatus()
#### Request
```json
{
  "method":"getStatus",
  "params": {},
  "jsonrpc": "2.0",
  "id": "1"
}
```

#### Response
```json
{
  "id":"1",
  "jsonrpc":"2.0",
  "result":{
    "blockCount":30375,
    "knownBlockCount":30375,
    "lastBlockHash":"ffa6dda0facc4a7ea903b0cededd897d2f281ac09eb6f50b9fed840dfe5073b5",
    "peerCount":22
  }
}
```

### createAddress()
#### Request
```json
{
  "method":"createAddress",
  "params": {},
  "jsonrpc": "2.0",
  "id": "1"
}
```

#### Response
```json
{
  "id":"1",
  "jsonrpc":"2.0",
  "result":{
    "address":"Xun3sfN5CzibcUqXqmqMFngzwaPS15iEySg2SVggQPArXzDVpJCHM5yhHPSR7X6nPG5DVpjrpNJ2Jg7Ej4DV3xgL5PEsCwEgBs"
  }
}
```

## Use Cases

### Wallet Management
- Create new wallets
- Delete existing wallets
- Check wallet balances
- Monitor transactions

### Transaction Processing
- Send funds
- Receive payments
- Track transaction status
- Handle unconfirmed transactions

## Troubleshooting

### Common Issues
- Wallet synchronization problems
- RPC connection failures
- Transaction processing delays
- Resource allocation issues

### Diagnostic Tools
- Check system logs
- Verify network connectivity
- Monitor resource usage
- Test RPC endpoints

## Support
For technical assistance, contact:
- Email: support@ultranote.org
- GitHub Issues: [Report Issues](https://github.com/xun-project/UltraNote-RPC-PHP/issues)
