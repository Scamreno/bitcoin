// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef H_BITCOIN_SCRIPT_STANDARD
#define H_BITCOIN_SCRIPT_STANDARD

#include "script/script.h"
#include "script/interpreter.h"

#include <stdint.h>

class CScript;

static const unsigned int MAX_OP_RETURN_RELAY = 40;      // bytes

// Mandatory script verification flags that all new blocks must comply with for
// them to be valid. (but old blocks may not comply with) Currently just P2SH,
// but in the future other flags may be added, such as a soft-fork to enforce
// strict DER encoding.
//
// Failing one of these tests may trigger a DoS ban - see CheckInputs() for
// details.
static const unsigned int MANDATORY_SCRIPT_VERIFY_FLAGS = SCRIPT_VERIFY_P2SH |
                                                          SCRIPT_VERIFY_CHECKLOCKTIMEVERIFY;

// Standard script verification flags that standard transactions will comply
// with. However scripts violating these flags may still be present in valid
// blocks and we must accept those blocks.
static const unsigned int STANDARD_SCRIPT_VERIFY_FLAGS = MANDATORY_SCRIPT_VERIFY_FLAGS |
                                                         SCRIPT_VERIFY_STRICTENC |
                                                         SCRIPT_VERIFY_NULLDUMMY;

// For convenience, standard but not mandatory verify flags.
static const unsigned int STANDARD_NOT_MANDATORY_VERIFY_FLAGS = STANDARD_SCRIPT_VERIFY_FLAGS & ~MANDATORY_SCRIPT_VERIFY_FLAGS;

enum txnouttype
{
    TX_NONSTANDARD,
    // 'standard' transaction types:
    TX_PUBKEY,
    TX_PUBKEYHASH,
    TX_SCRIPTHASH,
    TX_MULTISIG,
    TX_NULL_DATA,
};

class CNoDestination {
public:
    friend bool operator==(const CNoDestination &a, const CNoDestination &b) { return true; }
    friend bool operator<(const CNoDestination &a, const CNoDestination &b) { return true; }
};

/** A txout script template with a specific destination. It is either:
 *  * CNoDestination: no destination set
 *  * CKeyID: TX_PUBKEYHASH destination
 *  * CScriptID: TX_SCRIPTHASH destination
 *  A CTxDestination is the internal data type encoded in a CBitcoinAddress
 */
typedef boost::variant<CNoDestination, CKeyID, CScriptID> CTxDestination;

const char* GetTxnOutputType(txnouttype t);

bool Solver(const CScript& scriptPubKey, txnouttype& typeRet, std::vector<std::vector<unsigned char> >& vSolutionsRet);
int ScriptSigArgsExpected(txnouttype t, const std::vector<std::vector<unsigned char> >& vSolutions);
bool IsStandard(const CScript& scriptPubKey, txnouttype& whichType);
bool ExtractDestination(const CScript& scriptPubKey, CTxDestination& addressRet);
bool ExtractDestinations(const CScript& scriptPubKey, txnouttype& typeRet, std::vector<CTxDestination>& addressRet, int& nRequiredRet);

CScript GetScriptForDestination(const CTxDestination& dest);
CScript GetScriptForMultisig(int nRequired, const std::vector<CPubKey>& keys);

#endif // H_BITCOIN_SCRIPT_STANDARD
