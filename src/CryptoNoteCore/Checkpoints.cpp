// Copyright (c) 2011-2017 The Cryptonote developers
// Copyright (c) 2014-2017 XDN developers
// Copyright (c) 2016-2017 BXC developers
// Copyright (c) 2017-2020 UltraNote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <iterator>

#include "Checkpoints.h"
#include "../CryptoNoteConfig.h"
#include "Common/StringTools.h"

using namespace Logging;

namespace CryptoNote {
//---------------------------------------------------------------------------
Checkpoints::Checkpoints(Logging::ILogger &log) : logger(log, "checkpoints") {}
//---------------------------------------------------------------------------
bool Checkpoints::add_checkpoint(uint32_t height, const std::string &hash_str) {
  Crypto::Hash h = NULL_HASH;

  if (!Common::podFromHex(hash_str, h)) {
    logger(ERROR) << "<< Checkpoints.cpp << " << "Incorrect hash in checkpoints";
    return false;
  }

  if (!(0 == m_points.count(height))) {
    logger(ERROR) << "<< Checkpoints.cpp << " << "Incorrect hash in checkpoints";
    return false;
  }

  m_points[height] = h;
  return true;
}
//---------------------------------------------------------------------------
bool Checkpoints::is_in_checkpoint_zone(uint32_t  height) const {
  return !m_points.empty() && (height <= (--m_points.end())->first);
}
//---------------------------------------------------------------------------
bool Checkpoints::check_block(uint32_t  height, const Crypto::Hash &h, bool &is_a_checkpoint) const {
  auto it = m_points.find(height);
  is_a_checkpoint = it != m_points.end();
  if (!is_a_checkpoint)
    return true;

  if (it->second == h) {    
    return true;
  } else {
    logger(Logging::ERROR) << "<< Checkpoints.cpp << " << "Checkpoint failed for height " << height
                           << ". Expected hash: " << it->second
                           << ", Fetched hash: " << h;
    return false;
  }
}
//---------------------------------------------------------------------------
bool Checkpoints::check_block(uint32_t  height, const Crypto::Hash &h) const {
  bool ignored;
  return check_block(height, h, ignored);
}
//---------------------------------------------------------------------------
bool Checkpoints::is_alternative_block_allowed(uint32_t  blockchain_height, uint32_t  block_height) const {
  if (0 == block_height)
    return false;

  if (block_height < blockchain_height - CryptoNote::parameters::CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW && !is_in_checkpoint_zone(block_height)) {
    logger(Logging::DEBUGGING, Logging::WHITE)
      << "<< Checkpoints.cpp << " << "Reorganization depth too deep : " << (blockchain_height - block_height) << ". Block Rejected";
    return false;
  }

  auto it = m_points.upper_bound(blockchain_height);
  if (it == m_points.begin())
    return true;

  --it;
  uint32_t  checkpoint_height = it->first;
  return checkpoint_height < block_height;
}

//---------------------------------------------------------------------------

std::vector<uint32_t> Checkpoints::getCheckpointHeights() const {
  std::vector<uint32_t> checkpointHeights;
  checkpointHeights.reserve(m_points.size());
  for (const auto& it : m_points) {
    checkpointHeights.push_back(it.first);
  }

  return checkpointHeights;
}



bool Checkpoints::load_checkpoints()
{
  for (const auto& cp : CryptoNote::CHECKPOINTS) 
  {
    add_checkpoint(cp.height, cp.blockId);    
  }
  return true;
}

bool Checkpoints::load_checkpoints_from_file(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file) {
		logger(Logging::ERROR, BRIGHT_RED) << "Could not load checkpoints file: " << fileName;
		return false;
	}
	std::string indexString;
	std::string hash;
	uint32_t height;
	while (std::getline(file, indexString, ','), std::getline(file, hash)) {
		try {
			height = std::stoi(indexString);
		} catch (const std::invalid_argument &)	{
			logger(ERROR, BRIGHT_RED) << "Invalid checkpoint file format - "
				<< "could not parse height as a number";
			return false;
		}
		if (!add_checkpoint(height, hash)) {
			return false;
		}
	}
	logger(Logging::INFO) << "Loaded " << m_points.size() << " checkpoints from "	<< fileName;
	return true;
}

}