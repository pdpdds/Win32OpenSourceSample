#ifndef __AUDIO_STREAM_H__
#define __AUDIO_STREAM_H__

// Microsoft Speech SDK Version 5.1 
// is at: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/SAPI51sr/html/Welcome.asp
// Download Speech SDK 5.1 from: http://www.microsoft.com/downloads/details.aspx?FamilyID=5e86ec97-40a7-453f-b0ee-6583171b4530&DisplayLang=en
// or direct: http://download.microsoft.com/download/speechSDK/SDK/5.1/WXP/EN-US/speechsdk51.exe
#include <sapi.h>

// STLSoft Headers
#include <comstl/util/initialisers.hpp>
#include <comstl/util/creation_functions.hpp>
#include <comstl/error/exceptions.hpp>
#include <stlsoft/smartptr/ref_ptr.hpp>
#include <winstl/conversion/char_conversions.hpp>

// Boost iostreams headers
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/concepts.hpp>  // sink


namespace audiostream
{

template < class SinkType >
class audio_sink: public SinkType
{
public:
   audio_sink()
   {      
      // Initialise the COM libraries
      static comstl::com_initializer coinit;                         

      // Get SAPI Speech COM object
      HRESULT hr;
      if(FAILED(hr = comstl::co_create_instance(CLSID_SpVoice, _pVoice))) 
         throw comstl::com_exception("Failed to create SpVoice COM instance", hr);                                                   
   } 

   // speak a wide character string
   std::streamsize write(const wchar_t* s, std::streamsize n)
   {
      // make a null terminated wstring.
      std::wstring str(s,n);                       
      // The actual COM call to Speak.
      _pVoice->Speak(str.c_str(), SPF_ASYNC, 0);   
      return n;
   }

   // speak a character string
   std::streamsize write(const char* s, std::streamsize n)
   {
      // make a null terminated string.
      std::string str(s,n);                        
      // convert to wide charater and call the actual speak method.
      return write(winstl::a2w(str), str.size());  
   }


   // Set the speach speed.
   void setRate(long n) { _pVoice->SetRate(n); }   

private:      
   // COM object smart pointer.
   stlsoft::ref_ptr< ISpVoice > _pVoice;             
};


template < class SinkType >
class audio_ostream_t: public boost::iostreams::stream< SinkType >, public SinkType
{
public:
   audio_ostream_t()
   {
      open(*this);
   }
};

typedef audio_ostream_t< audio_sink< boost::iostreams::sink > >  audio_ostream ;
typedef audio_ostream_t< audio_sink< boost::iostreams::wsink > > waudio_ostream;

} // namespace audiostream

#endif

