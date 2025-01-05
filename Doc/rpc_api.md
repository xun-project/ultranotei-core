# UltraNote Infinity RPC API Reference

## Introduction
The UltraNote Infinity RPC API provides programmatic access to wallet and blockchain functionality through JSON-RPC 2.0. This documentation covers both the single-wallet (`ultranoteiwallet`) and multi-wallet (`walletd`) implementations.

## API Endpoints
- Single Wallet: `http://127.0.0.1:8078/json_rpc`
- Payment Gateway: `http://localhost:8070/json_rpc`

## Authentication
All RPC calls require basic authentication:
```json
{
  "rpc-user": "username",
  "rpc-password": "password"
}
```

## API Methods

### Wallet Methods

#### getBalance()
Returns the current wallet balance.

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

### Address Methods

#### createAddress()
Creates a new wallet address.

**Request:**
```json
{
  "method": "createAddress",
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
    "address": "Xun3sfN5CzibcUqXqmqMFngzwaPS15iEySg2SVggQPArXzDVpJCHM5yhHPSR7X6nPG5DVpjrpNJ2Jg7Ej4DV3xgL5PEsCwEgBs"
  }
}
```

### Transaction Methods

#### sendTransaction()
Sends funds to one or more addresses.

**Request:**
```json
{
  "method": "sendTransaction",
  "params": {
    "addresses": ["Xun3ZtBPE7eYvz1Uokg9zg9m8UJsYdWFyEFT6Mmk4snXgMeaSfAQRGKhHPSR7X6nPG5DVpjrpNJ2Jg7Ej4DV3xgL5PEsCMBnGV"],
    "anonymity": 2,
    "fee": 10000,
    "transfers": [
      {
        "amount": 900000,
        "address": "Xun3qFybMCTcqzexD68QMjDoHUDUqUCWEJ82svTJ5vtbYF652s7o3njYe2AvyWtSL2iiiELby9mGH6dkQZryga4P4fVVUmGVMk"
      }
    ]
  },
  "jsonrpc": "2.0",
  "id": "1"
}
```

## Error Handling
All errors follow the JSON-RPC 2.0 error format:
```json
{
  "code": -32601,
  "message": "Method not found",
  "data": {
    "method": "invalidMethod"
  }
}
```

## Versioning
The API follows semantic versioning (v1.0.0). Breaking changes will increment the major version number.

## Rate Limits
- 100 requests per second
- 1000 requests per minute

## Security Considerations
- Always use HTTPS for production environments
- Restrict RPC access to trusted IPs
- Use strong authentication credentials
- Regularly rotate API keys
