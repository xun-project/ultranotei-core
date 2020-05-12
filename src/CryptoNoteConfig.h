// Copyright (c) 2011-2017 The Cryptonote Developers
// Copyright (c) 2017-2018 The Circle Foundation & Conceal Devs
// Copyright (c) 2018-2019 Conceal Network & Conceal Devs
// Copyright (c) 2018-2020 UltraNote Network & UlraNote Devs
//
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdint>
#include <initializer_list>
#include <map>

namespace CryptoNote {
namespace parameters {

const uint64_t CRYPTONOTE_MAX_BLOCK_NUMBER = 500000000;
const size_t   CRYPTONOTE_MAX_BLOCK_BLOB_SIZE = 500000000;
const size_t   CRYPTONOTE_MAX_TX_SIZE = 1000000000;
const uint64_t CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0x2de638; /* Xuni address prefix */
const size_t   CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW = 10; /* 20 minutes */
const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT = 60 * 60 * 2; /* two hours */
const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V1 = 360; /* changed for LWMA3 */
const uint64_t CRYPTONOTE_DEFAULT_TX_SPENDABLE_AGE = 10; /* 20 minutes */

const size_t   BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW = 30;
const size_t   BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V1 = 11; /* changed for LWMA3 */

const uint64_t MONEY_SUPPLY = UINT64_C(21000000000000); /* max supply: 21M (Consensus II) */

const size_t   CRYPTONOTE_REWARD_BLOCKS_WINDOW = 100;
const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE = 100000; /* size of block in bytes, after which reward is calculated using block size */
const size_t   CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE = 600;
const size_t   CRYPTONOTE_DISPLAY_DECIMAL_POINT = 6;

const uint64_t POINT = UINT64_C(1000); 
const uint64_t COIN = UINT64_C(1000000); /* smallest atomic unit */
const uint64_t MINIMUM_FEE = UINT64_C(1000); /* 0.001000 xuni */
const uint64_t MINIMUM_FEE_BANKING = UINT64_C(1000); /* 0.001000 xuni */
const uint64_t DEFAULT_DUST_THRESHOLD = UINT64_C(10); /* 0.000010 xuni */  

const uint64_t DIFFICULTY_TARGET = 120; /* two minutes */
const uint64_t EXPECTED_NUMBER_OF_BLOCKS_PER_DAY = 24 * 60 * 60 / DIFFICULTY_TARGET; /* 720 blocks */
const size_t   DIFFICULTY_WINDOW = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY; 
const size_t   DIFFICULTY_WINDOW_V1 = DIFFICULTY_WINDOW;
const size_t   DIFFICULTY_WINDOW_V2 = DIFFICULTY_WINDOW;
const size_t   DIFFICULTY_WINDOW_V3 = 60; /* changed for LWMA3 */
const size_t   DIFFICULTY_BLOCKS_COUNT = DIFFICULTY_WINDOW_V3 + 1; /* added for LWMA3 */
const size_t   DIFFICULTY_CUT = 60; /* timestamps to cut after sorting */
const size_t   DIFFICULTY_CUT_V1 = DIFFICULTY_CUT;
const size_t   DIFFICULTY_CUT_V2 = DIFFICULTY_CUT;
const size_t   DIFFICULTY_LAG = 15; 
const size_t   DIFFICULTY_LAG_V1 = DIFFICULTY_LAG;
const size_t   DIFFICULTY_LAG_V2 = DIFFICULTY_LAG;


static_assert(2 * DIFFICULTY_CUT <= DIFFICULTY_WINDOW - 2, "Bad DIFFICULTY_WINDOW or DIFFICULTY_CUT");

const uint64_t DEPOSIT_MIN_AMOUNT 							= 1 * COIN; 
const uint32_t DEPOSIT_MIN_TERM 							= 22000; /* one month */
const uint32_t DEPOSIT_MAX_TERM 							= 1 * 12 * 22000; /* one year */
const uint64_t DEPOSIT_MIN_TOTAL_RATE_FACTOR 				= 0; /* constant rate */
const uint64_t DEPOSIT_MAX_TOTAL_RATE 						= 3; /* legacy deposits */

static_assert(DEPOSIT_MIN_TERM > 0, "Bad DEPOSIT_MIN_TERM");
static_assert(DEPOSIT_MIN_TERM <= DEPOSIT_MAX_TERM, "Bad DEPOSIT_MAX_TERM");
static_assert(DEPOSIT_MIN_TERM * DEPOSIT_MAX_TOTAL_RATE > DEPOSIT_MIN_TOTAL_RATE_FACTOR, "Bad DEPOSIT_MIN_TOTAL_RATE_FACTOR or DEPOSIT_MAX_TOTAL_RATE");

const uint64_t MULTIPLIER_FACTOR 							 = 10; /* legacy deposits */
const uint32_t END_MULTIPLIER_BLOCK 						 = 3600; /* legacy deposits */

const size_t   MAX_BLOCK_SIZE_INITIAL = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE * 10;
const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR 		 = 100 * 1024;
const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_DENOMINATOR 		 = 365 * 24 * 60 * 60 / DIFFICULTY_TARGET;

const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS 	 = 1;
const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS = DIFFICULTY_TARGET * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS;

const size_t   CRYPTONOTE_MAX_TX_SIZE_LIMIT = (CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE / 4) - CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE; /* maximum transaction size */
const size_t   CRYPTONOTE_OPTIMIZE_SIZE 					 = 100; /* proportional to CRYPTONOTE_MAX_TX_SIZE_LIMIT */

const uint64_t CRYPTONOTE_MEMPOOL_TX_LIVETIME 				 = (60 * 60 * 12); /* 12 hours in seconds */
const uint64_t CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME = (60 * 60 * 24); /* 23 hours in seconds */
const uint64_t CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL  = 7; /* CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL * CRYPTONOTE_MEMPOOL_TX_LIVETIME  = time to forget tx */

const size_t   FUSION_TX_MAX_SIZE = CRYPTONOTE_MAX_TX_SIZE_LIMIT * 2;
const size_t   FUSION_TX_MIN_INPUT_COUNT 					= 12;
const size_t   FUSION_TX_MIN_IN_OUT_COUNT_RATIO 			= 4;

const uint64_t UPGRADE_HEIGHT 								= 1;			
const uint64_t UPGRADE_HEIGHT_V2							= 1;
const uint64_t UPGRADE_HEIGHT_V3 							= 2; 

const unsigned UPGRADE_VOTING_THRESHOLD = 90; // percent
const size_t   UPGRADE_VOTING_WINDOW = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY; 
const size_t   UPGRADE_WINDOW = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY; 

static_assert(0 < UPGRADE_VOTING_THRESHOLD && UPGRADE_VOTING_THRESHOLD <= 100, "Bad UPGRADE_VOTING_THRESHOLD");
static_assert(UPGRADE_VOTING_WINDOW > 1, "Bad UPGRADE_VOTING_WINDOW");

const char     CRYPTONOTE_BLOCKS_FILENAME[] 				  = "blocks.dat";
const char     CRYPTONOTE_BLOCKINDEXES_FILENAME[] 			  = "blockindexes.dat";
const char     CRYPTONOTE_BLOCKSCACHE_FILENAME[] 			  = "blockscache.dat";
const char     CRYPTONOTE_POOLDATA_FILENAME[] 				  = "poolstate.bin";
const char     P2P_NET_DATA_FILENAME[] 						  = "p2pstate.bin";
const char     CRYPTONOTE_BLOCKCHAIN_INDICES_FILENAME[]       = "blockchainindices.dat";
const char     MINER_CONFIG_FILE_NAME[]                       = "miner_conf.json";

} // parameters

const uint64_t START_BLOCK_REWARD 							  = (UINT64_C(5) * parameters::COIN); // start reward (Consensus I)
const uint64_t FOUNDATION_TRUST 							  = (UINT64_C(2100000) * parameters::COIN); // 2.1M locked funds to secure network  (Consensus II)
const uint64_t MAX_BLOCK_REWARD 							  = (UINT64_C(10) * parameters::COIN); // max reward (Consensus I)
const uint64_t REWARD_INCREASE_INTERVAL 					  = (UINT64_C(21900)); // aprox. 1 month (+ 0.4 xuni increment per month)

const char     CRYPTONOTE_NAME[] 							  = "ultranotei";
const char     GENESIS_COINBASE_TX_HEX[] 					  = "010a01ff0001c096b102029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd08807121018defaf357ba96274bf7c0aa07cca61025e710e829efb9c5d9af9f067b8882990";
const uint32_t GENESIS_NONCE = 10000;
const uint64_t GENESIS_TIMESTAMP 							  = 1587360548; // 20200420

const uint8_t  TRANSACTION_VERSION_1 						  = 1; 
const uint8_t  TRANSACTION_VERSION_2 						  = 2; 

const uint8_t  BLOCK_MAJOR_VERSION_1 						  = 1; // (Consensus I) 
const uint8_t  BLOCK_MAJOR_VERSION_2 						  = 2; // (Consensus II) 
const uint8_t  BLOCK_MAJOR_VERSION_3 						  = 3; // (Consensus III)

const uint8_t  BLOCK_MINOR_VERSION_0 = 0; // important
const uint8_t  BLOCK_MINOR_VERSION_1 = 1; // important

const size_t   BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT = 10000; // by default, blocks ids count in synchronizing
const size_t   BLOCKS_SYNCHRONIZING_DEFAULT_COUNT = 128; // by default, blocks count in blocks downloading
const size_t   COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT = 1000;

const int      P2P_DEFAULT_PORT = 42000;
const int      RPC_DEFAULT_PORT = 43000;


/* P2P Network Configuration Section - This defines our current P2P network version
and the minimum version for communication between nodes */
const uint8_t  P2P_CURRENT_VERSION = 1;
const uint8_t  P2P_MINIMUM_VERSION = 1;
const uint8_t  P2P_UPGRADE_WINDOW  = 2;

const size_t   P2P_LOCAL_WHITE_PEERLIST_LIMIT = 1000;
const size_t   P2P_LOCAL_GRAY_PEERLIST_LIMIT = 5000;

const size_t   P2P_CONNECTION_MAX_WRITE_BUFFER_SIZE = 64 * 1024 * 1024;   // 64MB
const uint32_t P2P_DEFAULT_CONNECTIONS_COUNT = 8;
const size_t   P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT = 70;            // percent
const uint32_t P2P_DEFAULT_HANDSHAKE_INTERVAL = 60;                       // seconds
const uint32_t P2P_DEFAULT_PACKET_MAX_SIZE = 50000000;                    // 50000000 bytes maximum packet size
const uint32_t P2P_DEFAULT_PEERS_IN_HANDSHAKE = 250;
const uint32_t P2P_DEFAULT_CONNECTION_TIMEOUT = 5000; 					  // 5 seconds
const uint32_t P2P_DEFAULT_PING_CONNECTION_TIMEOUT = 2000; // 2 seconds
const uint64_t P2P_DEFAULT_INVOKE_TIMEOUT = 60 * 2 * 1000; // 2 minutes
const size_t   P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT = 5000; // 5 seconds
const char     P2P_STAT_TRUSTED_PUB_KEY[] = "e9a5f0d30549afde49c9bfbaa52ac60afdc46304642b460a9ea34bf7a4ef7061";

// Seed Nodes
const std::initializer_list<const char*> SEED_NODES  = {
   "45.63.58.18:42000",
   "139.180.136.6:42000",
   "155.138.140.208:42000",
   "45.32.206.198:42000",
};

struct CheckpointData {
  uint32_t height;
  const char* blockId;
};

#ifdef __GNUC__
__attribute__((unused))
#endif

// Blockchain Checkpoints:
// {<block height>, "<block hash>"},
const std::initializer_list<CheckpointData> CHECKPOINTS  = {

 {0, "b87145f2d92e8dbee77e431dfe1f6ce92fd91077e065cb042f751d3a9541ba62"},
 {10, "47266c029a66efef9441ecde446c1ffa708f14580b91f692003ea45c078daa0a"}, 
 {20, "bbeef61e486a5b8699ab3e2edec15f1f00a72f11d8de0bc067337385e8ed7556"},
 {30, "fc083b78a196f5ae901baff88963006c9469fcd6306d0b0795313c703f2b2bd5"},
 {40, "6a921a436e70725eb1707aad006a786b8684681a91da2aa9231014307253c2a1"},
 {50, "45a1ad0925d254b111a364d4b46d9d136dd6c00a53fae6d04647a176833e6da5"},
 {60, "d162c6cef7e68d5175090df27268a174daf3848f3bc82eb0d6a3606ed2aeedb0"},
 {70, "2b64a1a6be069d0ffaeae53fac29a5d8a83b0057778816553a52f7c70dc4a245"},
 {80, "ba9d57272bf56617c0c409ee012bd8ca8f5aa6c1914162a52e09842bac708962"},
 {90, "3916c7326773311765c45a98127a69e54aafb891c7540bcadbcf6c12190e81c1"},
 {100, "ef7252d1ceaa43713e0339d916e2474fad981607f13f6d83ccc9041e061dfb4b"},
 {110, "3f0c1dc9350e2ea45ae27acbbd3f1bc3f2b2c363cbcfdfc8ec3f778642a294a1"},
 {120, "331f969bfa4238721602c84aa31908021e676dfec0e5bb1b78f9de42cd69a9ed"},
 {130, "20890464f93f892fe86f4865c82452367e9b1f8b5dc887a56d6ff890da0f5e8e"},
 {140, "e8ba18c126d5b8e2105f9c269ed0a0be96e8b1b6c40f0b91f00b3a3c54e059aa"}, 
 {150, "612271d7a372bb601095675a42f16f81196f5777df8f676b8e4278821d1189f6"},
 {160, "db618b49614323ca9e01c69c921bbb5d1deac33382c21b62754f580470b048aa"}, 
 {170, "98fec282d0701a7a6594afff6350fc50fe3cf4c7d86565b4e619b33e0e65e777"},
 {180, "14472ea39ebb674b6ce99e52339c68956a822b9e3c5d08e656f46244da52f701"},
 {190, "f6a99f2f3f2a5311dfb7798bb6facb6df2f66438520d3c1bd095cdc47158c082"},
 {200, "a6cd9ca92ef9628cef526f1e3dfb46eb6dab9c30f411afc57aee1c36a4bd30ce"},
 {210, "456dae0a03c567acbac87d180babb5b1cdca7fc542cb0f084ab51d00fecdef39"}, 
 {220, "7cee7e4b1372cfbdd1ef730f84414acbb83ed6a18ffc8e30d0040dc120c29401"},
 {230, "e8e9ce482429f05c9ccafa5f7a24aafd08b40dcda429f3a257554bc925fe0407"},
 {240, "9ec6bdebc31e66faa83aa49bc214b76989f2fbb6a173f00ca6a68e1c6335eb68"},
 {250, "c921a79ca4e1ce3b6829f4afdc595a0d9a11c79bef56e161465c3db77a1dc7b6"},
 {260, "5369c2feccbae767d5999bb9b5b0273cb7348ebf2cf801af8c5e749f618987fe"},
 {270, "7584012e230d4e5ef359d4208ab15ef85df3c58c463af74d2fbe1df9584b19c1"}, 
 {280, "6c2b76ab17014fc59143a67d9deb24abb5ab1cd61b114f475aa7809d7a02f021"},
 {290, "b2cb28b6ae23454a8ff62658abdfcd8e7ba633529fa1df9d9fd4515db68b157a"},
 {300, "6f1e09e9f0aa81fa17c7ee2f0e048d4075bde3977977c1a63cf2945ccb14980d"},
 {310, "5106f42d45922d86b668fa0434035940a804195f9d1705dfe6d1b0e85720ffae"},
  



};


} // CryptoNote

#define ALLOW_DEBUG_COMMANDS