# UltraNote Infinity Integration Guide

## Overview

UltraNote Infinity provides multiple integration options for online services and applications. Depending on how your system accounts for user funds, there are two primary ways to integrate UltraNote into your application to receive and send transactions.

## Wallet Solutions

UltraNote Infinity distributes two wallet solutions:

### 1. `ultranoteiwallet` - Single Wallet Daemon
- Provides a single wallet address
- Suitable for services that manage funds in a single account
- Simple JSON RPC interface
- Ideal for basic payment processing

### 2. `walletd` - Multi-Wallet Payment Gateway
- Provides multiple wallet addresses (one per user)
- Suitable for exchanges, marketplaces, and multi-user systems
- Advanced JSON RPC API with user management
- Ideal for complex payment systems

## Choosing the Right Solution

### Use `ultranoteiwallet` when:
- You need a simple payment processing system
- Your service manages all funds in a single account
- You don't require per-user wallet addresses
- You need basic transaction capabilities

### Use `walletd` when:
- You operate an exchange or marketplace
- You need separate wallet addresses for each user
- You require advanced user management features
- You need multi-user transaction capabilities

## Alternative Integration Options

If you need a multi-wallet solution (one wallet per user), consider:
- **[Payment Gateway Documentation](./paymentgate.md)** - Complete guide for `walletd`
- **[PHP RPC Wrapper](https://github.com/xun-project/UltraNote-RPC-PHP)** - PHP library for integration
- **[WordPress Plugin](https://github.com/xun-project/UltraNote-WP-PaymentGateway)** - WordPress payment gateway

---

## Using `ultranoteiwallet` (Single Wallet)

### Overview
`ultranoteiwallet` provides a JSON RPC service that allows applications to interact with a single UltraNote wallet. This is the recommended approach if your application cannot call C++ methods directly.

### Setup Process

#### 1. Generate a New Wallet
```bash
./ultranoteiwallet --generate-new-wallet walletname
```

#### 2. Run the Node Daemon
```bash
./ultranoteid --config-file /etc/XUNI/node.conf
```

#### 3. Start the RPC Server
```bash
./ultranoteiwallet --wallet-file walletname --password 1234 --rpc-bind-port 8078 --rpc-user test --rpc-password 1234
```

The RPC server will now be running at `http://127.0.0.1:8078/json_rpc`

### JSON RPC Methods

#### Method: `getbalance()`
Get current wallet balance.

**Request:**
```json
{
	"method": "getbalance", 
	"params": {},
	"jsonrpc": "2.0", 
	"id": "1"
}
```

**Response:**
```json
{
	"id": "1",
	"jsonrpc": "2.0",
	"result": {
		"available_balance": 1000,
		"balance": 1000,
		"locked_amount": 0,
		"unlocked_balance": 0
	}
}
```

### Use Cases

#### Case 1: Using Payment ID for User Identification

When receiving payments from users, you can use Payment IDs to identify which user sent the payment. This is particularly useful for:
- E-commerce transactions
- User account deposits
- Service payments

**Implementation Steps:**
1. Generate a unique Payment ID for each transaction
2. Provide the Payment ID to the user for their transaction
3. Monitor incoming transactions with that Payment ID
4. Credit the user's account when the transaction is confirmed

#### Case 2: Mass Payouts

For sending payments to multiple users:
1. Prepare a list of recipient addresses and amounts
2. Use the `sendTransaction` method with multiple transfers
3. Monitor transaction status and confirmations
4. Update user accounts accordingly

### Security Considerations

- Always use secure RPC credentials (avoid default usernames/passwords)
- Run RPC services on localhost or behind a firewall
- Use HTTPS reverse proxy for external access
- Regularly update wallet software and dependencies
- Implement proper error handling and transaction monitoring

### Best Practices

1. **Transaction Confirmation**: Wait for multiple confirmations before considering transactions final
2. **Error Handling**: Implement robust error handling for network issues and API failures
3. **Logging**: Maintain detailed logs of all RPC calls and transactions
4. **Backups**: Regularly backup wallet files and configuration
5. **Monitoring**: Implement system monitoring for daemon status and performance

### Troubleshooting

Common issues and solutions:
- **Connection refused**: Check if daemon is running and firewall settings
- **Authentication failed**: Verify RPC username and password
- **Wallet not synced**: Ensure blockchain is fully synchronized
- **Insufficient funds**: Check available balance vs locked balance

### Next Steps

For more advanced integration scenarios, refer to:
- [Payment Gateway Documentation](./paymentgate.md) for multi-wallet support
- [API Reference](../README.md#api-integration) for complete method list
- [Systemd Setup](./systemd.md) for production deployment

## Support

For integration assistance:
- Check the main [README.md](../README.md) for general guidance
- Review existing GitHub issues for common problems
- Contact the development team for specific integration questions
