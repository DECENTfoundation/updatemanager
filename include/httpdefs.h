#pragma once

namespace Update
{

   enum Http_SecChannel : unsigned int
   {
      SEC_CHANNEL_NO_SSL = 0,
      SEC_CHANNEL_SSL_WITH_CLIENT_CERT,
      SEC_CHANNEL_SSL_WITHOUT_CLIENT_CERT,

   };
} //end namespace