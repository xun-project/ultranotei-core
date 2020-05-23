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

const uint64_t MONEY_SUPPLY = UINT64_C(21000000000000); /* max supply: 21M */

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
const uint64_t DEPOSIT_MAX_TOTAL_RATE 						= 3; /* 3% per annual */

static_assert(DEPOSIT_MIN_TERM > 0, "Bad DEPOSIT_MIN_TERM");
static_assert(DEPOSIT_MIN_TERM <= DEPOSIT_MAX_TERM, "Bad DEPOSIT_MAX_TERM");
static_assert(DEPOSIT_MIN_TERM * DEPOSIT_MAX_TOTAL_RATE > DEPOSIT_MIN_TOTAL_RATE_FACTOR, "Bad DEPOSIT_MIN_TOTAL_RATE_FACTOR or DEPOSIT_MAX_TOTAL_RATE");

const uint64_t MULTIPLIER_FACTOR 							 = 10; /* Early Adopters */
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

const uint64_t START_BLOCK_REWARD 							  = (UINT64_C(5) * parameters::COIN); // start reward
const uint64_t FOUNDATION_TRUST 							  = (UINT64_C(2100000) * parameters::COIN); // 2.1M locked funds to secure network
const uint64_t MAX_BLOCK_REWARD 							  = (UINT64_C(10) * parameters::COIN); // max reward after 12 months
const uint64_t REWARD_INCREASE_INTERVAL 					  = (UINT64_C(22000)); // aprox. 1 month (+ 0.4 xuni increment per month)

const char     CRYPTONOTE_NAME[] 							  = "ultranotei";
const char     GENESIS_COINBASE_TX_HEX[] 					  = "010a01ff0001c096b102029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd08807121018defaf357ba96274bf7c0aa07cca61025e710e829efb9c5d9af9f067b8882990";
const uint32_t GENESIS_NONCE = 10000;
const uint64_t GENESIS_TIMESTAMP 							  = 1587360548; // 20200420

const uint8_t  TRANSACTION_VERSION_1 						  = 1; 
const uint8_t  TRANSACTION_VERSION_2 						  = 2; 

const uint8_t  BLOCK_MAJOR_VERSION_1 						  = 1; // (V1) 
const uint8_t  BLOCK_MAJOR_VERSION_2 						  = 2; // (V2) 
const uint8_t  BLOCK_MAJOR_VERSION_3 						  = 3; // (V3)

const uint8_t  BLOCK_MINOR_VERSION_0 = 0; 
const uint8_t  BLOCK_MINOR_VERSION_1 = 1; 

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
 {100, "ef7252d1ceaa43713e0339d916e2474fad981607f13f6d83ccc9041e061dfb4b"},
 {200, "a6cd9ca92ef9628cef526f1e3dfb46eb6dab9c30f411afc57aee1c36a4bd30ce"},
 {300, "6f1e09e9f0aa81fa17c7ee2f0e048d4075bde3977977c1a63cf2945ccb14980d"},
 {400, "f9cf88eaed1568fb0bba3582c5c2d123a77c955408fc7c59603184978073c428"},
 {500, "7ad64134fdd48645766f9969cd920ff70697984396c252fb5caa41d4e866d507"},
 {600, "559f4f955abac0863d8cd86b853a8c06f370f647b4f269c90be5a289d92586e3"},
 {700, "2c9c9caa77d73bb15518d9575fdd1474cb42e4b5bd0af7189347237758e9dca5"},
 {800, "2500fdf6296bc969084a4030f9dd984191fe434ad2ec507ed91b88387b72c42f"},
 {900, "fcd7cda2222017c71531648c23748c9f9ca5c6641a7c8a5648ae0d3621055fd6"},
 {1000, "083729e0be52246f19d1da23fbe5f96b4e3948e5effd705179d38131041a96d5"},
 {1100, "2013046da15f95438051245de36c18a0fd255602e5853f59fcbd7eb5320bee7e"},
 {1200, "5cb33176955a9d0e95da9344388c7c7ce8faa10d873b6741cf1686a7e79690c7"},
 {1300, "09aca2fa9db0f60fbffa93fdeba6877b025028b634a0dea86bef6e7a167eb6fe"},
 {1400, "47ebd593c67fff7f0e6f012e9bdb95c4c391cd6a4065c8e346c8e2af5cb730b9"},
 {1500, "b302f1c1a178edbca58f417b550d85509f50c580978466c354cdce47bbd4bc3b"},
 {1600, "75ea12dd8fd48e0580d57240ffd7e8c2fb70e51ba3b22d56d8b93cbe55dcfa3a"},
 {1700, "ff28e386bfce53947e20447bdcea88f9a10d44a29d90aee819613e4e596a721b"},
 {1800, "b96f3cd89b5a55d8b558e799b226d4e6f23cd56de8f7ef3ae58183318cedb298"},
 {1900, "2efdf8be3418fab6bd23a0bd16c89cfc16eb1387f8353714940876a8833dd8c9"},
 {2000, "f3433995e2203a1908133622ef0802b152955c7a1ce605f1e9251bc817e38b09"},
 {2100, "2625f8b4a50a89d299d07132ac0615630906ecbbbb6b55e11121698573c6091a"},
 {2200, "527b725f307b4b8074ea6ccfa73932a1cb07ec08bf80e72677aae9941fac44b1"},
 {2300, "d1294ea4fbb10c97c713c6c50a2b0650a9d0422eed03dab202b2c9555529788e"},
 {2400, "4c2e6418f1df84d11f32edc2ff62452230d5ee16a27e649d5146cd7d76bd4d18"},
 {2500, "267af42ac6239a316701eab3cbacb2d2b60fd38ce5a107092e304456672d8be5"},


};


} // CryptoNote

#define ALLOW_DEBUG_COMMANDS