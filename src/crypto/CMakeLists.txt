set(CRYPTO_SOURCES
  aux_hash.c
  chacha8.c
  crypto-ops-data.c
  crypto-ops.c
  crypto.cpp
  cryptonight.cpp
  groestl.c
  hash.c
  keccak.c
  oaes_lib.c
  random.c
  tree-hash.c
)

set(CRYPTO_HEADERS
  aux_hash.h
  blake256.h
  chacha8.h
  cn_aux.hpp
  coin_algos.hpp
  crypto-ops.h
  crypto.h
  cryptonight.h
  cryptonight.hpp
  generic-ops.h
  groestl.h
  groestl_tables.h
  hash-ops.h
  hash.h
  initializer.h
  keccak.h
  oaes_config.h
  oaes_lib.h
  random.h
  randomize.h
  rctTypes.h
  soft_aes.hpp
  secure_memory.h
  secure_random.h
  key_manager.h
)

add_library(crypto ${CRYPTO_SOURCES} ${CRYPTO_HEADERS})
target_link_libraries(crypto common)
set_property(TARGET crypto PROPERTY FOLDER "libs")
