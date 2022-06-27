

  
//#include <SD.h>

//#include <SPI.h>
//#include <IPAddress.h>

//#include <IniFile.h>








#if 0
void printErrorMessage(uint8_t e,bool eol=true)
{
	switch(e)
	{
		case IniFile::errorNoError:			Serial.print("no error");				break;
		case IniFile::errorFileNotFound:	Serial.print("file not found");			break;
		case IniFile::errorFileNotOpen:		Serial.print("file not open");			break;
		case IniFile::errorBufferTooSmall:	Serial.print("buffer too small");		break;
		case IniFile::errorSeekError:		Serial.print("seek error");				break;
		case IniFile::errorSectionNotFound:	Serial.print("section not found");		break;
		case IniFile::errorKeyNotFound:		Serial.print("key not found");			break;
		case IniFile::errorEndOfFile:		Serial.print("end of file");			break;
		case IniFile::errorUnknownError:	Serial.print("unknown error");			break;

		default:							Serial.print("unknown error value");	break;
	}
  
	if(eol)
    	Serial.println();
}
#endif

void ReadConfigFile(void)
{
	const size_t bufferLen=64;
	char buffer[bufferLen];
	const char *filename="/tracker.ini";
	
	

}

void UseConfigDefaults(void)
{
  
}
