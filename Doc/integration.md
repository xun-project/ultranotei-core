# UltraNote Infinity Integration Guide

## Overview
This guide provides instructions for integrating UltraNote Infinity into online services and applications.

## Integration Options

### Single-Wallet Solution (ultranoteiwallet)
- Provides one wallet address
- Suitable for services managing funds in a single account
- Uses JSON RPC interface

### Multi-Wallet Solution (walletd)
- Provides multiple wallet addresses
- Suitable for services managing funds per user
- Uses JSON RPC interface
- Supports PHP wrapper and WordPress plugin

## Single-Wallet Integration

### Prerequisites
1. Generate new wallet:
   ```sh
   ./ultranoteiwallet --generate-new-wallet walletname
   ```
2. Start node daemon:
   ```sh
   ./ultranoteid --config-file /etc/XUNI/node.conf
   ```
3. Start wallet RPC server:
   ```sh
   ./ultranoteiwallet --wallet-file walletname --password 1234 --rpc-bind-port 8078 --rpc-user test --rpc-password 1234
   ```

### RPC Methods

#### getbalance()
##### Request
```json
{
  "method":"getbalance",
  "params": {},
  "jsonrpc": "2.0",
  "id": "1"
}
```

##### Response
```json
{
  "id":"1",
  "jsonrpc":"2.0",
  "result":{
    "available_balance":1000,
    "balance":1000,
    "locked_amount":0,
    "unlocked_balance":0
  }
}
```

## Multi-Wallet Integration

### Prerequisites
1. Install walletd
2. Create configuration file
3. Generate wallet container:
   ```sh
   walletd --config=/etc/paymentgate.conf --generate-container
   ```
4. Start walletd:
   ```sh
   walletd --config=/etc/paymentgate.conf
   ```

### Available Integration Methods
1. Direct JSON RPC calls
2. PHP wrapper
3. WordPress plugin

## Best Practices

### Security
- Use HTTPS for RPC communication
- Implement proper authentication
- Regularly rotate credentials
- Monitor system logs

### Performance
- Optimize database configuration
- Use proper indexing
- Implement caching
- Monitor resource usage

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
