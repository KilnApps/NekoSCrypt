// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>
#include <limits>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <ctime>
#include <iostream>
#include <arith_uint256.h>

using namespace std;

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Bloomberg 1/Dec/2021 South African Scientists Sound Caution on Omicron Case Severity";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 2100000;
        consensus.BIP34Height = 344407;
        consensus.BIP34Hash = uint256S("0x9ca8e68e34ccdeeb35a52176155a7d524c8216a82450d696abcfda340129e271");
        consensus.BIP65Height = 2120750;
        consensus.BIP66Height = 2120750;
        consensus.CSVHeight = 2157120; // 118ebe35057df2a42cfbbea8eb330e6f39da9e4b087202ce509b7a6bf7666f1e
        consensus.SegwitHeight = 2157120; // 118ebe35057df2a42cfbbea8eb330e6f39da9e4b087202ce509b7a6bf7666f1e
        consensus.MinBIP9WarningHeight = 2157120 + 20160; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powNeoScryptLimit = uint256S("0000003fffff0000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // three and a half weeks
        consensus.nPowTargetSpacing = 2.5 * 60; //two and a half minutes
        consensus.checkpointPubKey = "0425165000270c45a2df329bb6cc179782e1ddcdd9afc6240b5f465d49b8f480635c43e6b194e98f69e9827fa7a62d4791ee96c9fa90ec11e45af5d6b2dbb76774";
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 15120; // 75% of 20160
        consensus.nMinerConfirmationWindow = 20160;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1638410635; // December 1, 2021
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1672462800; // December 31, 2022

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000a90c3cad43e87a7");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xe29c854a0a54ac2c3f6b97e0416a0a7f17df6f80ecf4fa6eb3faa7ec1fceffb6"); //3275900

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x93;
        pchMessageStart[1] = 0x91;
        pchMessageStart[2] = 0xa0;
        pchMessageStart[3] = 0xf8;

        // End of the bridge from old to new pchMessageStart
        pchMessageStartOld[0] = 0xfb;
        pchMessageStartOld[1] = 0xc0;
        pchMessageStartOld[2] = 0xb6;
        pchMessageStartOld[3] = 0xdb;

        nDefaultPort = 5364;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 3;
        m_assumed_chain_state_size = 1;

        consensus.nForkOne = 33000;
        consensus.nForkTwo = 87948;
        consensus.nForkThree = 204639;
        consensus.nForkFour = 432000;
        consensus.nTimeLimit = 2236032;
        consensus.nNeoScryptFork = 1414346265;

        genesis = CreateGenesisBlock(1638410635, 884508, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        // assert(consensus.hashGenesisBlock == uint256S("0xe8df435b636e9a8605500fa8c887d6d3d203cb669ff7327fdcce18b7d6cff02b"));
        // assert(genesis.hashMerkleRoot == uint256S("0x9280f91f66072971e557fe5910ac83caf2cd1cda5b0de5d0a07f52afe83bdda8"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.

        /* feathercoin's seed nodes
        vSeeds.emplace_back("dnsseed.feathercoin.com");
        vSeeds.emplace_back("dnsseed1.feathercoin.com");
        vSeeds.emplace_back("dnsseed.alltheco.in");
        vSeeds.emplace_back("dnsseed.bushstar.co.uk");
        */
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,14);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,142);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xBC, 0x26};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xDA, 0xEE};

        bech32_hrp = "ns";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;

        checkpointData = {
            {
                {  0, uint256S("0xe03da9c79859ccc0b6af6bcfd5d1c71d5ce11f74bc7c12f3b83f5b2bc6a2a77f")},
            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 e29c854a0a54ac2c3f6b97e0416a0a7f17df6f80ecf4fa6eb3faa7ec1fceffb6
            /* nTime    */ 0,
            /* nTxCount */ 0,
            /* dTxRate  */ 0
        };
    }
};

/**
 * Testnet (v5)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2100000;
        consensus.BIP34Height = 16;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 1;
        consensus.MinBIP9WarningHeight = 501; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powNeoScryptLimit = uint256();
        consensus.nPowTargetTimespan = 60;
        consensus.nPowTargetSpacing = 60;
        consensus.checkpointPubKey = "0421c27bb6580b05dcda1f47e59274489f094a3e85d96bbc38d5befd10eee97397ec8a93b6d8d79e8370239a8f39adf66322b41dafe83066bbcee6144e4c41a699";
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 375; // 75% for testchains
        consensus.nMinerConfirmationWindow = 500;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1637808495; // November 24, 2022
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1672462800; // December 31, 2022

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); //1354312

        pchMessageStart[0] = 0xcb;
        pchMessageStart[1] = 0xb9;
        pchMessageStart[2] = 0x98;
        pchMessageStart[3] = 0xe5;

        pchMessageStartOld[0] = 0xda;
        pchMessageStartOld[1] = 0xaf;
        pchMessageStartOld[2] = 0xa5;
        pchMessageStartOld[3] = 0xba;

        nDefaultPort = 15763;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        consensus.nForkTwo = 0;
        consensus.nForkThree = 0;
        consensus.nForkFour = 0;
        consensus.nTimeLimit = 100;
        consensus.nNeoScryptFork = 1581518000;

        genesis = CreateGenesisBlock(1637802296, 77668, 0x1f00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();


        

        // assert(consensus.hashGenesisBlock == uint256S("0xc3ab8326676d5a2f015e2449c093244c6852ac102a485275c4593b550eb9ff5f"));
        // assert(genesis.hashMerkleRoot == uint256S("0x81455057ca8dd66a853ed3b92fb9d81d43cfe38d098417dec0ab5a89286b209c"));

        vFixedSeeds.clear();
        vSeeds.clear();
        
        // nodes with support for servicebits filtering should be at the top
        /* feathercoin's seed nodes
        vSeeds.emplace_back("testnet-explorer2.feathercoin.com");
        vSeeds.emplace_back("testnet-dnsseed.feathercoin.com");
        */
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,197);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "nst";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;


        checkpointData = {
            {
                
            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 545 c889c0fb27bf7c669cff1cf9407f768cde2a084e1dc527baa6dadbed9b22bf29
            /* nTime    */ 1581570241,
            /* nTxCount */ 547,
            /* dTxRate  */ 0.01200996055444148
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in functional tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in functional tests)
        consensus.CSVHeight = 432; // CSV activated on regtest (Used in rpc activation tests)
        consensus.SegwitHeight = 0; // SEGWIT is always activated on regtest unless overridden
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powNeoScryptLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // three and a half days
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.checkpointPubKey = "04f7908c00071227442382eeeb699be0cf4837ce63c6dfb8efd63e240ba27906f30b708934ad1831f26713437c945eae05e42d2f2400baf3310b60caf0d7e39cab";
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xa5;
        pchMessageStart[1] = 0xdb;
        pchMessageStart[2] = 0xd9;
        pchMessageStart[3] = 0xfe;

        pchMessageStartOld[0] = 0xda;
        pchMessageStartOld[1] = 0xaf;
        pchMessageStartOld[2] = 0xa5;
        pchMessageStartOld[3] = 0xba;

        nDefaultPort = 18446;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateActivationParametersFromArgs(args);

        consensus.nForkTwo = 0;
        consensus.nForkThree = 0;
        consensus.nForkFour = 0;
        consensus.nTimeLimit = std::numeric_limits<int>::max();
        consensus.nNeoScryptFork = 1515840635;

        genesis = CreateGenesisBlock(1637808495, 3, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();


        

        // assert(consensus.hashGenesisBlock == uint256S("0x81a5ef89ff0edf99dd3a94f9b42c58a8a1836e0a7c954542e63886246d72cb9c"));
        // assert(genesis.hashMerkleRoot == uint256S("0x81455057ca8dd66a853ed3b92fb9d81d43cfe38d098417dec0ab5a89286b209c"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;

        checkpointData = {
            {
                
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,34);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,35);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,36);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "nsrt";
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (gArgs.IsArgSet("-segwitheight")) {
        int64_t height = gArgs.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }

    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
