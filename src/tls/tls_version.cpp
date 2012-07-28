/*
* TLS Protocol Version Management
* (C) 2012 Jack Lloyd
*
* Released under the terms of the Botan license
*/

#include <botan/tls_version.h>
#include <botan/tls_exceptn.h>
#include <botan/parsing.h>

namespace Botan {

namespace TLS {

std::string Protocol_Version::to_string() const
   {
   const byte maj = major_version();
   const byte min = minor_version();

   if(maj == 3 && min == 0)
      return "SSL v3";

   if(maj == 3 && min >= 1) // TLS v1.x
      return "TLS v1." + std::to_string(min-1);

   if(maj == 254) // DTLS 1.x
      return "DTLS v1." + std::to_string(255 - min);

   // Some very new or very old protocol (or bogus data)
   return "Unknown " + std::to_string(maj) + "." + std::to_string(min);
   }

bool Protocol_Version::is_datagram_protocol() const
   {
   return major_version() == 254;
   }

bool Protocol_Version::operator>(const Protocol_Version& other) const
   {
   if(this->is_datagram_protocol() != other.is_datagram_protocol())
      throw TLS_Exception(Alert::PROTOCOL_VERSION,
                          "Version comparing " + to_string() +
                          " with " + other.to_string());

   return m_version > other.m_version;
   }

bool Protocol_Version::known_version() const
   {
   // Don't include DTLS yet here as we can't actually process it
   return (m_version == Protocol_Version::SSL_V3 ||
           m_version == Protocol_Version::TLS_V10 ||
           m_version == Protocol_Version::TLS_V11 ||
           m_version == Protocol_Version::TLS_V12);
   }

bool Protocol_Version::supports_negotiable_signature_algorithms() const
   {
   return (m_version == Protocol_Version::TLS_V12 ||
           m_version == Protocol_Version::DTLS_V12);
   }

bool Protocol_Version::supports_explicit_cbc_ivs() const
   {
   return (m_version == Protocol_Version::TLS_V11 ||
           m_version == Protocol_Version::TLS_V12 ||
           m_version == Protocol_Version::DTLS_V10 ||
           m_version == Protocol_Version::DTLS_V12);
   }

}

}
