/* Copyright (c) 2009 maidsafe.net limited
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   *  Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
   *  Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
   *  Neither the name of the maidsafe.net limited nor the names of its
    contributors may be used to endorse or promote products derived from this
    software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MAIDSAFE_COMMON_RSA_H_
#define MAIDSAFE_COMMON_RSA_H_

#include <string>

#ifdef __MSVC__
#  pragma warning(push, 1)
#  pragma warning(disable: 4702)
#endif
#include "cryptopp/channels.h"
#include "cryptopp/ida.h"
#include "cryptopp/rsa.h"
#ifdef __MSVC__
#  pragma warning(pop)
#endif

#include "boost/function.hpp"
#include "boost/filesystem/path.hpp"

#include "maidsafe/common/crypto.h"
#include "maidsafe/common/error.h"


namespace maidsafe {

namespace rsa {

typedef CryptoPP::RSA::PrivateKey PrivateKey;
typedef CryptoPP::RSA::PublicKey PublicKey;
typedef std::string ValidationToken, Identity, PlainText, Signature, CipherText;
typedef std::function<void(PublicKey,
                           ValidationToken)> GetPublicKeyAndValidationCallback;
typedef std::function<void(Identity,
                           GetPublicKeyAndValidationCallback)>
    GetPublicKeyAndValidationFunctor;
typedef std::function<bool(Identity,                          // NOLINT (Fraser)
                           PublicKey,
                           ValidationToken)> ValidatePublicKeyFunctor;
typedef std::function<bool(PlainText, Signature, PublicKey)> ValidateFunctor;  // NOLINT (Fraser)
struct Keys {
 public:
  enum { kKeySize = 2048 };
  Keys() : identity(), private_key(), public_key(), validation_token() {}
  Identity identity;
  PrivateKey private_key;
  PublicKey public_key;
  ValidationToken validation_token;  // certificate, additional signature etc.
};

Keys GenerateKeyPair();

std::string Encrypt(const crypto::NonEmptyString& plain_text, const PublicKey& public_key);

std::string Decrypt(const CipherText& data, const PrivateKey& private_key);

std::string Sign(const PlainText& data, const PrivateKey& private_key);

std::string SignFile(const boost::filesystem::path& filename,
             const PrivateKey& private_key);

bool CheckSignature(const PlainText& data, const Signature& signature, const PublicKey& public_key);

bool CheckFileSignature(const boost::filesystem::path& filename,
                       const Signature& signature,
                       const PublicKey& public_key);

std::string EncodePrivateKey(const PrivateKey& key);

std::string EncodePublicKey(const PublicKey& key);

PrivateKey DecodePrivateKey(const std::string& private_key);

PublicKey DecodePublicKey(const std::string& public_key);

// check decoded keys were the same as encoded and pub key not replaced
bool CheckRoundtrip(const PublicKey& public_key, const PrivateKey& private_key);

bool ValidateKey(const PrivateKey& private_key, unsigned int level = 2U);

bool ValidateKey(const PublicKey& public_key, unsigned int level = 2U);

bool Validate(const PlainText& plain_text, const Signature& signature, const PublicKey& public_key);

bool MatchingPublicKeys(const PublicKey& public_key1, const PublicKey& public_key2);

bool MatchingPrivateKeys(const PrivateKey& private_key1, const PrivateKey& private_key2);

bool SerialiseKeys(const Keys& keys, std::string& serialised_keys);

bool ParseKeys(const std::string& serialised_keys, Keys& keys);

}  // namespace rsa

namespace asymm = rsa;

}  // namespace maidsafe

#endif  // MAIDSAFE_COMMON_RSA_H_
