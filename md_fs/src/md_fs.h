#ifndef _MD_FS_H_
  #define _MD_FS_H_

  #define MD_FS_VERSION "0.0.1"

  // Maximum length for filename, excluding NULL char
  #define MD_MAX_FILENAME_LEN 31

  #include "SPIFFS.h"
  #include "FS.h"
  #include "IPAddress.h"

  class SPIFFSIniFileState;

  class SPIFFSIniFile
    {
      public:
      	enum error_t {
      		errorNoError = 0,
      		errorFileNotFound,
      		errorFileNotOpen,
      		errorBufferTooSmall,
      		errorSeekError,
      		errorSectionNotFound,
      		errorKeyNotFound,
      		errorEndOfFile,
      		errorUnknownError,
      	};

  	    static const uint8_t maxFilenameLen;

      	// Create an SPIFFSIniFile object. It isn't opened until open() is called on it.
      	SPIFFSIniFile(const char* filename, const char* mode = "r", bool caseSensitive = false);
      	~SPIFFSIniFile();

      	inline bool open(void); // Returns true if open succeeded
      	inline void close(void);
      	inline bool isOpen(void) const;

      	inline error_t getError(void) const;
      	inline void clearError(void) const;

	    	inline const char* getMode(void) const; // Get the file mode (FILE_READ/FILE_WRITE)
      	inline const char* getFilename(void) const; // Get the filename asscoiated with the ini file object
	      bool               validate(char* buffer, size_t len) const;

      	// Get value from the file, but split into many short tasks. Return
      	// value: false means continue, true means stop. Call getError() to
      	// find out if any error
      	bool getValue(const char* section, const char* key,
                      char* buffer, size_t len, SPIFFSIniFileState &state) const;

      	// Get value, as one big task. Return = true means value is present

      	// in buffer
        bool getValue(const char* section, const char* key, char* buffer, size_t len) const;
      	// string (not buffer)
        bool getValue(const char* section, const char* key, char* buffer, size_t len, char *value, size_t vlen) const;
	    	// boolean value
        bool getValue(const char* section, const char* key, char* buffer, size_t len, bool& b) const;
      	// integer value
      	bool getValue(const char* section, const char* key, char* buffer, size_t len, int& val) const;
      	// uint16_t value
      	bool getValue(const char* section, const char* key, char* buffer, size_t len, uint16_t& val) const;
      	// Get a long value
      	bool getValue(const char* section, const char* key, char* buffer, size_t len, long& val) const;
        bool getValue(const char* section, const char* key, char* buffer, size_t len, unsigned long& val) const;
      	// float value
      	bool getValue(const char* section, const char* key, char* buffer, size_t len, float& val) const;

      	bool getIPAddress(const char* section, const char* key, char* buffer, size_t len, uint8_t* ip) const;
	      bool getIPAddress(const char* section, const char* key, char* buffer, size_t len, IPAddress& ip) const;

      	bool getMACAddress(const char* section, const char* key, char* buffer, size_t len, uint8_t mac[6]) const;

      	// Utility function to read a line from a file, make available to all
      	//static int8_t readLine(File &file, char *buffer, size_t len, uint32_t &pos);
      	static error_t readLine(File &file, char *buffer, size_t len, uint32_t &pos);
      	static bool isCommentChar(char c);
      	static char* skipWhiteSpace(char* str);
      	static void removeTrailingWhiteSpace(char* str);

      	bool getCaseSensitive(void) const;
      	void setCaseSensitive(bool cs);

      protected:
      	// True means stop looking, false means not yet found
      	bool findSection(const char* section, char* buffer, size_t len, SPIFFSIniFileState &state) const;
      	bool findKey(const char* section, const char* key, char* buffer, size_t len, char** keyptr, SPIFFSIniFileState &state) const;

      private:
      	char _filename[MD_MAX_FILENAME_LEN];
      	const char* _mode;
      	mutable error_t _error;
      	mutable File _file;
      	bool _caseSensitive;
    };



class SPIFFSIniFileState
{
public:
	SPIFFSIniFileState();

private:
	enum {funcUnset = 0,
		  funcFindSection,
		  funcFindKey,
	};

	uint32_t readLinePosition;
	uint8_t getValueState;

	friend class SPIFFSIniFile;
};


#endif

