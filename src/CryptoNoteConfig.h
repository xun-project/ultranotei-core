// Copyright (c) 2011-2017 The Cryptonote Developers
// Copyright (c) 2017-2018 The Circle Foundation & Conceal Devs
// Copyright (c) 2018-2019 Conceal Network & Conceal Devs
// Copyright (c) 2018-2022 UltraNote Infinity developers
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
const uint64_t MAX_BLOCK_REWARD 							  = (UINT64_C(10) * parameters::COIN); // max reward after aprox. 20 months
const uint64_t REWARD_INCREASE_INTERVAL 					  = (UINT64_C(22000)); // aprox. 1 month (+ 0.25 xuni increment per month)

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
const size_t COMMAND_RPC_GET_OBJECTS_MAX_COUNT = 1000;

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
   "45.63.58.18:42000",     // seed1.ultranote.org
   "139.180.136.6:42000",   // seed2.ultranote.org
   "62.171.173.164:42000",  // seed3.ultranote.org
   "207.244.227.124:42000", // seed4.ultranote.org
   "209.126.13.23:42000",   // node1.ultranote.org
   "75.119.146.144:42000",   // node2.ultranote.org

   
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
 {1000, "083729e0be52246f19d1da23fbe5f96b4e3948e5effd705179d38131041a96d5"},
 {2000, "f3433995e2203a1908133622ef0802b152955c7a1ce605f1e9251bc817e38b09"},
 {3000, "79421be1ef1a0e590e8749decdcc551cbc4125265d6d939be61b4d6c46ca4b37"},
 {4000, "af43cc12cbb8ec47ea08245ccde29ff3a29f68f6f295bb2034437f0c101a67b1"},
 {5000, "40a860cfb519f2ce479d04f8c5c1872f8f79fa5903254071742678369484421b"},
 {6000, "b94a1065693123f16615d207523a03bed5f484b7b0030f62f030efc7f01a4ebf"},
 {7000, "b67e9057cf96b0aca0bd575a002d4376402ce47196b68b42b8ee0780e119ba45"},
 {8000, "40b206b2eaf8926f6e6282cd0984080718a14e41979a25df7d1f760316f25119"},
 {9000, "f68164192390ec50a4c1a0824de4996cf6a2a238dbdacd2aae1f6520b2850962"},
 {10000, "ebdb99611acc9b6d2f0c9fffae5a467e8a420e29189449f7f8e09b0f5fc1e1c4"},
 {11000, "2e772769a517cd302bf94e97803817c413df2976edd5ecaf1ece84df6ac5c082"},
 {12000, "45ce9d710a845b0fd00497031aa470e02cef889269dc3412ab0af8437018b9cb"},
 {13000, "5fa5db9c4a76ce32691c7631bd2573b9e0d3c5ff0ef23f9252a33f253cd611e9"},
 {14000, "82a80f5732141ac2352bcdb22ded56e507810caee8128c66e7e128a8e85a3105"},
 {15000, "7ce3bd474371b1999f6fc9c326d1736772500c382798ce6e7d43d894832da9ec"},
 
 // version 1.0.2
 
 {16000, "8a256c9125a60419f2fb14de6c2e2f3ffc208a70a2f848d9690d9cd582d39031"},
 {17000, "bee5fe19e0242c6d87256a5efaa0430bdd38c576d678bdbd52a5b00c55130a8d"},
 {18000, "4e4bea66c6d12f226684fbaf87eb9f6bdedaf2158957b52b5d1cf189a7058bb2"},
 {19000, "91dbf1cebcf0323397928a2e4473d5e555e76ee9234dd1a1b1890160b14da708"},
 {20000, "4f75d7ea3c116e6ebbd37edc472b79bb9551e9be2801564a1b4be9e2689dcd82"},
 {21000, "b38812e690d5fd738d5bf737990cf4597e62f718cc304574443e58d18297f508"},
 {22000, "05f0e7480053c167fd7eb8600ebad707abdc167c06b4e5fd31c763184ae5cdf0"},
 {23000, "1f266cadefe8d02e439e9acdfdf790eeb1c5d6e987d4414905376afd4df14c33"},
 {24000, "ad86d75b0bdda9fa0c6781c6ee9d9db0566c6ab09fd602a323d4b6b710d4e5f8"},
 {25000, "bb8bd3c62ced4018f0b7c8894142353a64d2d415bb0e816fec77b36f1ab6e128"},
 {26000, "3c24d434abeda12154bda48947f931d39bec5a7e4e3e17b62528815e891903dd"},
 {27000, "1ebe055542822e6501a2b9db9b9bb17da617e7d91f740b9df6e6f2e2739a2204"},
 {28000, "d095e0b1bbc371019f8eedab0f42122bf024df87078db14585decad951cc62d4"},
 {29000, "ccc67b0d94e201223f6d0d05cf6d78b867f94a43c3d7020880181f55c62fb299"},
 {30000, "8f54018897d4a2109d3f54810d5c36030b1d597de9e6dc04c9dd22f472664ed2"},
 {31000, "20ddbafdd31afe17a5515c30479506380b0be4bbd9f72e225925d0d906eeba31"},
 {32000, "69559b8f877a83b34d7f8392d27748410535d0fcbca90a194e654b1af09b9605"},
 {33000, "e5a0f221ae112677527272f92257e69386990a6ac9221fa7f05decd5b8f9b17a"},
 {34000, "2bbb74acc78aed072d5e08444ec8e5e041eace6037e051f7c484dac18dc87727"},
 {35000, "184b593a15438b0136413c5da48cb6ba12aeac21af81cb732eecedf0ec09815a"},
 {36000, "aad80b1f81ba905232f92cb78b2c23222fc4e062332f72fafde4f87c12e20bf6"},
 {37000, "2b09b4d6789ddd4874f4ed50806f785aec200cdd6acc636d7a5e0a155d1c93e6"},
 {38000, "1c4d1fb1e26d8c54320b279217babc3bfb4bd31507c84cbf24c6e74615315c9d"},
 {39000, "9a97ed51be3416f968c86b81cf0fb3b6de68b25e35ee7f024098b9d004085006"},
 {40000, "e9dddac678a339e4b59f8fe647a5bc7b263f1ebfa92a56b44087935b11e16696"},
 {41000, "8547a1c018b9152e56c84d61ae0fe84fc516e56b6c13de809c39ffc892938581"},
 {42000, "738d55a9e4a070bedfb089d2701da357c8fe81c17e43cae4bfc5f7a525b8fe1e"},
 {43000, "2d16a1374dd740db480e2990458ae673762bedbe70d8b00a420fe679eb746676"},
 {44000, "68254d51e921dddca8bd004b1aaf0368ccd152075b91d6b1ae50a46c998259f7"},
 {45000, "c3fe93ce44f6a83368d4ccdcfda4318f86f81dff7a18e5cd368a835b56cae9de"},
 {46000, "7e6a3fb703468f8dfbddd38f371aae01c4a7566e0f00ba32df43c66ade28a803"},
 {47000, "c9cebc75a9817ff2a345cd6d8d470a1d17148826d613a7535aada3901e06dd7a"},
 {48000, "a8a0d644171a4e581590f6907a7e29971f1fda9cbc3da2ec3b0a83100ec8ac0c"},
 {49000, "1075ff9963624c03e39d39883b9707689fc46cb87c45bb069c8a2245226ba0dc"},
 {50000, "cf75b17ea35abd19285c9a6f3de145efa3a77f583ebf6629460aa170d72a602e"}, 
 {51000, "e6d6210f2ee96083deec431bd730159450cccd7ed07772628648a536177b1467"},
 {52000, "45b8f979085e09da5d3f7755ff1d941e923e2f6a311013330feeb6b826f3c290"},
 {53000, "71d6c5bf2f8a840738598409876b223e2cc21502a7c15e198a69472e65836297"},
 {54000, "bea9246d94ccdf8909213d371d07d2ba599149a51aabe63a202021266d7a961e"}, 
 {55000, "96d09e12de9391fb28f026783fc684b8b3ae4f651a54633aa3071fdd6d59c507"}, 
 {56000, "643f62e472768f49f6d92031222f38a879c33943825421ffdbcbe6953be93016"},
 {57000, "dabeb5bd0c4daf3fc8f0e79a85feba9437bc8d72f1d65073b74c4f3298e245de"},
 {58000, "eb73d94b6cbbafdcb496ad1c3bd39849946b3ca100e449671d3994453cdaf564"},
 {59000, "a8328ff6a67b65f15f074f4465e88a6f11826aa7fd95ffd795c5a0a9574ec9db"},
 {60000, "603bb733a2797c59b8c034fa30c19eeb878433d4e6614283aee54f9f43c7583c"},
 {61000, "dc8090fa182fa14e06c32c45a9330f7c82a6e5ef67af2d0b4839c0c1fec86b1e"},
 {62000, "8e35b860cef8295931c327d8e267387491d0cbdd3253784bec9a194a97c4ec90"},
 {63000, "bba41d4ae67e585dc381fbd13cfdef9cc2e3fb371e4b8c705cffc327695b0924"},
 {64000, "e2377f67708a838e8f4f0a01be6d9bd7a075358f9575ad000ad42399d586bd68"},
 {65000, "62f627d96692f565b908e878a8649a6dfc888065e890d318e9e5cc839ff18250"},
 {66000, "e2a4043720a8b1dba7cda4c34f6b66065c6959ab9d25e90bf35d869660499d67"},
 {67000, "73976a287cbc37f2355e81578263224cfc9048ddd5ac25bca455473f51d26e11"},
 {68000, "88bd77491bd0d7d40d443b6042de1742d30f0701bed927eab23c64ec8de62a13"},
 {69000, "cdab9953c77232739d6dcb050661fa24a07fb1143e13b8e3d04fd3c83f74d06d"},
 {70000, "aa878b2c1c94a858162753af7bb55183d640479daa363ce21aceb3ca96dbf227"},
 {71000, "a3a3e0ca80470abbc23b21558cedf009fe6443873fcf74a645349283900638ef"},
 {72000, "020fc507d4762639a21c03c2fb52786cfa2ad4e11a02155b909f0b614324ade8"},
 {73000, "d73632bca4decb7c8d3cedd476dd05dbe365a9c074efa7279694a8d591c2042f"},
 {74000, "a047ddc34432e20cdc58830c1205227682b2e589a26e702cbd1a1fc50c685320"},
 {75000, "9b5c7ff36adee46df7befae7796c200252254a89061d485085d0ce34e2b2e6f2"},
 {76000, "a4644c80cff6e7da6087b294b14171dba7a5efab80a02053f5d1d9fae469828e"},
 {77000, "72ac6bac1c0c40eb061a826923aa4b4e87be0f289267cb057329419bf9447d00"},
 {78000, "8036e7ecffa0975037dfceb3efeea7f71c2c3e0150825333ef65e63edd06d694"},
 {79000, "da86d2ae83f93b3eb74edcca010ce0fdbe35390a64ece3d2a8666e0e60d83344"},
 {80000, "5ae8ed79e1b7a16ea30810db4c0182b105a39acc36c1f161966d3b60df4da7d1"},
 {81000, "c42110a5152ab0aa1e758d0649a3f7e1347792778af151cfa21a1c969920c260"},
 {82000, "00cad19ccf8bf0f555376ca2bdaa4e21ee9eee3fd6764445496535b7b377dfcc"},
 {83000, "55c343db70bdbe11ed0fed7a232a9d0fe93d3746633838143671ad05762d9bd0"},
 {84000, "8a49d7a01dba7a25a91036ef3bebe229968ddadaed361364fd9e98ee734c9e93"},
 {85000, "c73a5adeda8b0aef396bdeaeca547fb7ac914cc8c525183f69f7131d5ae426cb"},
 {86000, "c54373bdffeed582be84d71c42880bac316cbccf8139ddba1d48168071aa4a97"},
 {87000, "7dddcefb2c08f2d74f3d1ffef8f15d9b6e71b555475d424b145b17ef9b7bf609"},
 {88000, "c43fb141d9f6390f0dbc92ea08c569def12d032bc1278022159575660b880c2e"},
 {89000, "997ff021fcaf3b3d7d62710bdf15fc868d054becad980747cbcbd264896149c9"},
 {90000, "e8fc93a8af535b2097229329a556154925805fe39b86877971d61c885420736c"},
 {91000, "868d584c5b7122e8ecb1a69408d2f927c3f591ddad7a71d7f790d8477e437a77"},
 {92000, "32885daf1e09b5ffa4b496ccb5777c06c96e12bea57216486a3ff3b8974a0646"},
 {93000, "f10032402c69313216648e16c97830a95c57c3408db095db5799233c4c46b33d"},
 {94000, "0b69cf2fee047bc04683be77e8e80afb017029fbfdd4a38596b3e1d35088da99"},
 {95000, "3a4c216432392e6816709fb8b7e9fd01cf0c75a5f04a08f4d9e0db55baceaf58"},
 
 // version 1.0.3
 
 {96000, "d621fff6b32ea3772c2e9a9c981bf041aadef91b420b6be9c06009d981beea74"},
 {97000, "b238395ccff2c1c8de6333184a30b89ba9ece03db2e504aa0a2ba68ff0a3c9ea"},
 {98000, "57fd61bebb86e69f986e84fdaabd102f5a71855bb347f7ac8ca8a4b6d7b6b3be"},
 {99000, "887ce0bd253a55b89caa386ae7671132dcf364d641cbd1d56453adf56885f577"},
 {100000, "5eeefd0eb470d743ae545da9ba9748ddb01adde5c558eed3e9557bc8cab10b11"},
 {101000, "3925979e3bb7f830fffdbfa4488a236a9498bb823e01b1108838f1f5fe8fea11"},
 {102000, "42b35774f4acd370cafab6c59c843afdb28d0b93e7de181432ed850dd0b1f3fe"},
 {103000, "3b53d864c9fd406cb1e10deefef54a668a19590d083fb05585194af14c852c2f"},
 {104000, "2b7daecda78687b352f93e1d53a52ac6b233e51b72d85d98efa79677820bc837"},
 {105000, "17b038ae21c51ced24d06c823ba1cecd3e91a103f792521aa41f1a7d9d0a7ce2"},
 {106000, "708ad0ffff902b3888b0d4104808ea4397474eb5ece6aff6358604dad94d000d"},
 {107000, "fadab3c80207bf7a1fd22eea0b4ee88bc2a3aed20ea554043491e6bf99772500"},
 {108000, "0b8fad8bcfbfa9f31f5abdf9b1ada3b2c15140537337ec5eda51d865c1e3ae92"},
 {109000, "5297bb25f5705b80d23a39a4a58d9a4cc67a4f61a2211fa87d2e35a368585bfb"},
 {110000, "4bf279248e25de854cb619a9b017d9fe6ab5bb7b3e00f1847fadab4292810f84"},
 {111000, "72b95ab8eb82d09f130a7d69b404b7f01a04ab66f7ed2c8255bc93704aebbaa3"},
 {112000, "6eb31ef44b314e74bcde3fcbc9de06a6207a01d2a95496c53fc3c0f67212d582"},
 {113000, "5e8463672b352bc49ecdfe8a8ebd2ffa92c266de73ed221413af28b4fa5a9817"},
 {114000, "7876c007810dd5bcc1f599786590010dc991b8c9534dc9050850902f3b21a5a8"},
 {115000, "6ae07c1432a9b2e25ac21c3045c6c4accf69247f34cb25c7dfeb636415ea6b2c"},
 {116000, "0ba3aaa30ecc7da9cad0ee8596580f32ace3ebf9f5ed86ed7adea4efe57dd4de"},
 {117000, "f041cdeeb26e0a7c3250b104e17a9aa50cb3dcf705121ed71501c8a8a0c3e669"},
 {118000, "e57ebb8b4f2b3139d60e59efcd632c787b4f5a2b841aa49f29930771db00fbfb"},
 {119000, "088a26eeea1031e80ad63f6312aef864e3105ab76d751c7d2f04e853850d6adf"},
 {120000, "23e6f3538d49bac32ec0a8aeab229d7a94b28b50bad3127a25c1b26eb3bf7f8d"},
 {121000, "b736ade61df8da1bba1875a56c81305f8df9b9d72455c42ede13e25d6d4f47bb"},
 {122000, "b3377a52b30a9a3ef3116ebf6c938eceb207cd7b51b616aec0929480c341ec44"},
 {123000, "36b175e22af9d19ec8e1e9a3ee25ad67ee4a53f498e2f37a89d96b93892acda9"},
 {124000, "49229b9db0ec3c667d343170ee386dba3739cdee60a98143b0f338ccc42a15e7"},
 {125000, "1a114331ecd6ced71a54f2edaf681e79ee8da3c8753f19d4e1f6a6df4dcae7a9"},
 
   // version 1.0.4
  
 {126000, "de7104d3ecf68e0823d5d7bf457b00b2a1f3e9b56d92dc0d2a767fa43bf864e0"},
 {127000, "3e130b190ebdd8a6a169c43408d76f94305a12e26811bb613c8fd2f4db4b6251"},
 {128000, "d5ae187601aa25bbddb834b555bab55707ee934f9f53523cdb49a3caf6700e8d"},
 {129000, "7c135b5bdd42a386c5e4e48c96664942075fa1ae37b8428dd2e0671847c88c04"},
 {130000, "4ab3ba0ffadc43a9dec8ed306e97e67a3da79045bf7e9bb7c9eefbc1a366426a"},
 {131000, "c06f6dc3d419ebe8c3a8a51caae01bc6b33ab774db54f46d2dd79590a32370e6"},
 {132000, "a2697fb7398c867ce733982790b71b9b6dfdd4f1fe131a9a9e0284c838907414"},
 {133000, "72a3b61c02aafa05f7438acab195217a1fdad01d22dda6d0d6d78b0b674774af"},
 {134000, "b9400dc2cb795e674c1619c7fe2537c33efd5cafe8605533d772f85a318b2b0c"},
 {135000, "254a521734e017bf62053a47fa2088e5da62648fcb1286805755af83a68a0a40"},
 {136000, "91124b2e63b42623de1ddcbd9ddc13306d39b468ddec9c9136c6f19d639c86bf"},
 {137000, "b667921b2e27201855222e318c064a0cee6e403d4cd2733a1686fab7069f7aeb"},
 {138000, "51bc7a0872c79a83030294eb4f3984de35171d561aff770b9a1642fa2b1d32c0"},
 {139000, "6efd9f68c3bac635c0e3a1c18a5c31ea49d45a1c97aeac0e3c058432c0f4e01b"},
 {140000, "a873697ad52faf4e8b141d4df1a222fce78721178bb388eaba3871720e6ff058"},
 {141000, "363d936c95b389773e0d9fbace608a22304e22c07db4b9dfdcd357f985e4d805"},
 {142000, "be320a3116f07e14d7baae660416d1e8895c113c65a869a868003ef9f2b92eab"},
 {143000, "07f4e21b554f62f8998d558167110f30a185a03f8cd4112da61ca8869e7625db"},
 {144000, "d0c150a1335dc0b0fb45ff2efe830a4116f57b731b9ae68669ecaeafc1f3a1bd"},
 {145000, "4fd197ef4194b7c3e58ae9808dadbdd89cf97f32654b61f909f4906cfc3a9fa0"},
 {146000, "0f962ad5cd51dcc2361a28a3c0c12926d9004b4446b373279b9a915db21cc52f"},
 {147000, "2076fcc2cb67f871974d20e780904c96e4dc79f89f2578694e925887b0eb91e7"},
 {148000, "df7831e711e046a018ede9f7732d556f157ec2625328150152d0dd28411a3443"},
 {149000, "54574678bd814965b2f56f466d2c5743dc56d08721efbdc47a574051c4df41ec"},
   
 // version 1.0.5

 {150000, "d329cbb22fccc433c9dc32c51773229d14c9445a7691ca5f52316386dbb676da"},
 {151000, "0dede7450217635a0f553f085f689b9a8e503f09d43bff90196812bd34473689"},
 {152000, "4e9b761be4aebcc3bdc127222a974c05d247b68544163315c69a7923c59e628a"},
 {153000, "1ba6edf6ca0ab5fdf79c0869b20c470fa9d2aaffff66d1cdc898f3da0b64b14e"},
 {154000, "077123b3f03592ac508340ff468cd38242ee28fb899dec92c5f419d653c6cb89"},
 {155000, "fa27bece29fc515ff54076d0db6dea5eb40bcd54720fa80bdcd9745177c3c23e"},
 {156000, "8e370cad60db6e43172bb55ee34da12fb7e133f144aaa97258e4235394c0c3f4"},
 {157000, "a8cfc24b3a91d1f3adf85413ecf0249065102ef48b1dd0eddad0349b18ef236f"},
 {158000, "d4c442a83e9b5a79d54fdfd325151a53dfba5a43d237cf21c8a70cd0d58f17fa"},
 {159000, "06aae97fc2058f113f7ef652de20ea95b519c9fd83518e4c374c8b7e873980b5"},
 {160000, "c4f185ec67610229349d6f2044c0e103274320d01879ebe7e39553aa436a6f67"},
 {161000, "9d343b8638acc537a2083ae6e655b50b36d90496009d6310971fa1029c095fa5"},
 {162000, "ea3b8561f86c24a3c2448527c0cd40a5e81e7e6a5751646c04de12cad44cb9bf"},
 {163000, "1f494bf3de88c64d8c308ec08f508ab7e1c1eba6d9a42f4d75210f488d12703f"},
 {164000, "f2c6abdaff3a6f7bb1dbe12ca71fc1eb37d116e83f525b1a1fc330af82dc1cf5"},
 {165000, "052d5ca63d21983877f51d2df010307418abfabd24b8cf137753d55e3b1465d8"},
 {166000, "d087e664deaf9d0d2638ecd695378bcdb13657526449673c26ca88cc6afcfcf1"},
 {167000, "bb4ad6188617e8995d696688bc6be147743a2ef37d12a1dc5a81d506a7e3300a"},
 {168000, "831e02277cfa063b2ec4f34eee96c8f5b862cd3402df4971412f8b2c2161a1a5"},
 {169000, "92556070aa72f2fb70cc14d9c71170247bc8367df770d7c8fb2fe7ce8df60384"},
 {170000, "adbf20f1e4ec05c0a470d43afb3963792932ec012279dac63ca45defbf8b2bcd"},
 {171000, "2635a60a13401da93993be9657d01869e20b39e089a4de55d217de45c367e84f"},
 {172000, "4d36c2ff6a7069adbd24ddd26e7308ec2d201ea1b2d93e7deec3742b37447271"},
 {173000, "4ce2abdb470ba74a4c8735264f1b5f320f4f36600daadc11a8962541a88d2fb1"},
 {174000, "8d94051dfde3accbfd602eb1aa6c7c0baccf76c599a0fa124c63ac7a0024ade2"},
 {175000, "f216eccf5d3289e03ca05f1bfb9d6c6fccc89b41be1b030c741489cf65285af0"},
 {176000, "cfab8ff54728a51e09fcae3b389043a961086a34679c52d39203155a77b69114"},
 {177000, "44a063855905f06c910c5f68a71ca0197c057d2c835c21b312757aab9d08b95a"},
 {178000, "be7629315381299e2c51a1da3dc82e89f00077891e27dfcfc5bcd0996fee3c57"},
 {179000, "6f172885885ceff2551b8583b601d98590ff4fc0d1853ee868f574c099cbff41"},
 {180000, "fbf26f6e728089c48246ec1b522ec8aac8f9d24837f19488b9dc4582164f0017"},
 {181000, "dbe3c094bd40758ecfd1841b0ef5d8a779b1c2a42c375d92ba1cb37e848ff1b2"},
 {182000, "4110288a04182b9795678dc0970cfd4658b28b6a24dc4dbce78345a84e5da25d"},
 {183000, "49ba4d71514a59e2d80e1670214c8e2823dbd7f39647da9850a12780174c5e99"},
 {184000, "556650d90df7e0319ff84506d218709b37450facdceafd8efe953b8d306c31cd"},
 {185000, "91066b39ef4fd407d6379367791532ccd1dc3aaa97c60e2fe9d2f546b6d020d0"},
 {186000, "7fa299470f2164b3b395b2f6b87d7cf14e7f4bdc5638956a275931bf69c9e503"},
 {187000, "3552f9dc09fb70276382a26fd344795335a6be90ce179f8d1fd3b10441fd2507"},
 {188000, "c7619f098ba777fcde31ef2672780baf76a68f87582b0b03b572d9a3ce9e97c6"},
 {189000, "dc30718357e8221e437c0189691f9d3235cbae8e8c26a81212661f3026974d50"},
 {190000, "1797b295d180a1facdc7fc38f45b54321eed17e7ff77a920db24230c45376e33"},
 {191000, "4405b57af8d86112e80da0d3a128bb2c05e0b2661ee030abb83f9946ae6f50c9"},
 {192000, "7ceda62d9263c22929bfdcf1198ce20a795d8caf49090f10a78c10f2c20e9bf2"},
 {193000, "bbbc3fdc82ea2a6a67534a34db45baaa186847a0fdef876b748b88c212b569aa"},
   
   // version 1.0.6

 {200000, "3332bde1baaa5190e5c91eea52dbd78a4ecc6501306a136cf4866ebbba50b4bc"},
 {210000, "96529cb8edd5164fca39ad9f1d826a0c3f4336ef15a529d159bcfa85a25bae70"},
 {220000, "be338117eb35037a6f4bb2a386e7f98649ed12d7ddf9c450c84f0ddd992c85a1"},
 {230000, "e0b5f0ae31f9ee364b1133d0c99d979529e9da36d607c38c2417d4920955f8ce"},
 {240000, "45dfcd2cd62ed39234369b6eccb38464b4e3cb03b7577f5e7f199300a8e7421d"},
 {250000, "0da31d1bbd3202b8622523a72b19dbd0d080294a819d5847a28d0dc46313545d"},
 {260000, "5120709409215bbe9b1b1820e7c1b3236a66280fca850873a433ed1185c9dbb7"},
 {270000, "f69e485637e9713f33e9992edfc40271acc2f53be03cfbd917b2d95f40dbdf20"},
 {280000, "653a1242ddc8224af4489f9fdabd155c52e712c23d8e79b8ddcae00f7421eae4"},
 {290000, "9583dd6ad6fe7dbf6179724584bbe637075d284e5d635875a3b184b3f9457b68"},
 {300000, "eb1fba52c0060248c929c375daaf9a2095e391fedf487d2092024a9e87ab15da"},
 {310000, "9dc11bdbc2a7cdb0185428a12797032c8dde578d38b07688f9476075ecbd0f45"},
 {320000, "13fb67045df17f278d575af8441784e0494b1f0b7e6b64d6592de848bdb03798"},
 {330000, "5daadf363a76d869031c83541cf712ce0183553da2859256a0dffa27f1df04a8"},
 {340000, "356de0f1d7df1579522c28dcc44ea99a32c61f599ceadf9c86c60b229c0b69c2"},
 {350000, "154aa25c550971459980920e56105be1ef7924ae0845f84e9694815f4b391557"},
 {360000, "e2d35ce96eb9baa32d05287573bcfff11ce4dcd917a977532ca9fce628812b86"},
 {370000, "10420ed107fb16d0c526efe5b62b94c85659b690dd329ad143e7cac11a835252"},
 {380000, "b86e7acab1293746d05c2d18998d8ff73dd9dfaf351bf12d24ce421d06a08251"},
 {390000, "8de2fcebafb964e47b83069bb78c94ba3b0c3e226ce13b27405d9cd0f70a4f49"},
 {400000, "26670043242de92a7144b1b973aeeff3fa68ef0e5483d66fd24c36b45cd6eb47"},
 {410000, "62ca9e2eb2ccfffd7e8a605ce37ab35ef7bdaf43902fa49cfae5c5ccbb018471"},
 
    // version 1.0.7
	
 {420000, "c123229377d6dbbc1c6957bb98d6f2c1732cff7f81796eed2237d172664a6a3c"},
 {430000, "bc537bb1395f87d56a7f0400bc00d9d6a4cdcb8bbf5bcf67c875bb9085d8fa81"},
 {440000, "56a7c12b2ee2e5ac246c4b9a581aa121f58a3f9f774cfe83847115406122a156"},
 {450000, "0d629e7058f4035b13dcb6110c3c9e768bdca7987c2ff95b4147c9e8f1fdc2fd"},
 {460000, "20f911c5e9815379ab1de9be7cbde71054f4936243e8d4a872dfc2a6912fe168"},
 {470000, "83ea954fcb7a9be466451f210341b210fa74497039490704c9953ae046e0531e"},
 {480000, "17c02822b35d70185703c28312ae985724d53156652ce1eeb0254c0d66c3d08f"},
 {490000, "1cfea44d9984281968f3940c400fa79756623652a97e0918ef211e9a040784ce"},
 {500000, "d9d03c6c3979e8d68a861c5f97823862121a1199be20610529c03ac8d348837c"},
 {510000, "aedde71dd6948f7ea03852311249061ef2015abc442149d86ee99571933213ed"},
 {520000, "40be007cc16b040c9ffb0dcf6d0f18920baa7d45e601d56fda4dfadbd0fc65ce"},
 {530000, "03f283ad28e87a293b18592eb439c27ece831ca419404bda1ba3017f7e2d58f1"},
 {540000, "e6bc97946a61292db81327d4c995f5fc2df54eb7d7a472dfa2b1dd39f9b0d865"},
 {550000, "fa7a2ebb6d00afc6df05bcd5f8352821c978d85d42f9f25975d0ea2749829520"},
 {560000, "53b84b36b49e8ea3dd684b301c598097ab80c81e2bd28a9eeabfc9144056149c"},
 {570000, "30b094db5599da4292542b8f52489fdb9488a7063e23b1c190ec409e534c1359"},
 {580000, "6c685dd9d873466f6970394f6872ce34320f51865a39da47fbec26a4f8f76761"},
 {590000, "4bc720f22bb5d34ef690c66a7e63c8b63caf929a53c6b102283759f2b28368bf"},
 {600000, "f0b1911cb45bbea0e1b1ac32d6536736c02b2d85a741b937a9dda46ed8574dd0"},
 {610000, "d45cb5405036e2685ef336a41b7296ba5499d86d5c8ae457f1c4475187f4ad2b"},
 {620000, "7b67f78b35b895eeaea34f1dfa09515ee205595e29ae6e7afac0da79cbc1b209"},
 {630000, "19c5b4a6ca89092dd70c36c8b92ce88db99ac2452768949635581877396869ac"},
 {640000, "d4627c0b35b4059a3b8cedb0e11e0ed2162ac4cb017fb52bd845031266b876a2"},
 {650000, "c9d7761fd75dd1f9d103315a3d2ff77e20ee15a3ce2f4bf1ef7eeca0fb6ed255"},
 
 
};


} // CryptoNote

#define ALLOW_DEBUG_COMMANDS
