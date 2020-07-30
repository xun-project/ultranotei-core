# WORK IN PROGRESS
## UltraNote Infinity For Online Services 

Depending on how your system accounts user funds there are two ways to integrate UltraNote into an application to receive and send transactions.

UltraNote distributes two wallet solutions: `ultranoteiwallet` and `walletd`. Both daemons are capable of recieving and sending mass payouts. The difference is that `walletd` provides multiple wallet addresses when `ultranotewallet` provides only one wallet address. 

In this document we'll show how to use  `ultranotewallet`. If you need a multi-wallet solution (one wallet per user) refer to [paymentgate documentation](./paymentgate.md), use the [PHP Wrapper](https://github.com/xun-project/UltraNote-RPC-PHP) or use the [WordPress Plugin](https://github.com/xun-project/UltraNote-WP-PaymentGateway).



### Using `ultranoteiwallet`

`ultranoteiwallet` provides *JSON RPC* service. At the time this is the only way to use wallet methods if your application cannot call `c++` methods directly. 


### Running

1. Before running `ultranoteiwallet` you need to generate a new wallet:

```sh
	~# ./ultranoteiwallet --generate-new-wallet walletname

```

2. Run the node daemon `ultranoteid`:

```sh
	~# ./ultranoteid --config-file /etc/XUNI/node.conf
```   

3. Run the wallet in RPC Server mode

```sh
	~# ./ultranoteiwallet  --wallet-file walletname --password 1234 --rpc-bind-port 8078 --rpc-user test --rpc-password 1234
``` 

Now the rpc server runs at `http://127.0.0.1:8078/json_rpc` and you can call following methods:


### Method: `getbalance()`
Get current balance

Request:
```json
{
	"method":"getbalance", 
	"params": {},
	"jsonrpc": "2.0", 
	"id": "1"
}
```


Response:
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


### Use cases

#### _Case 1:_ Using PaymentID 
