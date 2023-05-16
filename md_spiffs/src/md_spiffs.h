/* SPIFFS filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

// TODO append(), peek(), convert() ...

#ifndef _MD_SPIFFS_HPP_
  #define _MD_SPIFFS_HPP_
    //#include <stdio.h>
      //#include <string.h>
      //#include <sys/unistd.h>
      //#include <sys/stat.h>
      //#include <stdlib.h>
    #include "esp_err.h"
      //#include "esp_log.h"
      //#include "esp_spiffs.h"
    #include <SPIFFS.h>
    #include <md_defines.h>

    #define FFS_MAXLEN 50
    /*
        static const char *TAG = "SPIFFS";

        static const char *base_path = "/spiffs";
        char buffer[FFS_MAXLEN];
      */

    //*****************************************************************************
    class md_spiffs : public fs::SPIFFSFS
      {
        /* FS methods
          File open  (const char* path, const char* mode = FILE_READ);
          bool exists(const char* path);
          bool remove(const char* path);
          bool rename(const char* pathFrom, const char* pathTo);
          bool mkdir (const char *path);
          bool rmdir (const char *path);

          SPIFSFS methods
          bool begin(bool formatOnFail=false, const char * basePath="/spiffs",
                     uint8_t maxOpenFiles=10, const char * partitionLabel=NULL);
          bool format();
          size_t totalBytes();
          size_t usedBytes();
          void end();
          */

        public:
          esp_err_t init(md_spiffs* pffs, bool formatOnFail=false, const char * basePath="/spiffs",
                                          uint8_t maxOpenFiles=10, const char * partitionLabel=NULL);
          uint8_t   writeFile(char* filename, const char* text);
          uint8_t   deleteFile(const char* filename);
          uint8_t   renameFile(const char* filename_src, const char* filename_tar);
          uint8_t   readFile(const char* filename, uint16_t len, char* textBuf);
          void      clearBuffer();
          //uint8_t disableSpiffs();
        private:
          md_spiffs*  _pffs  = NULL;
          size_t      _total = 0; //
          size_t      _used  = 0;
      };
  extern md_spiffs  spiffs;
/*
    void app_main(void)
    {
        esp_vfs_spiffs_conf_t conf = {
          .base_path = "/spiffs",
          .partition_label = NULL,
          .max_files = 5,
          .format_if_mount_failed = true
        };

        initSpiffs(&conf);

        const char* text = "Hello World!\n";

        writeFile("/hello.txt", text);

        deleteFile("/foo.txt");

        renameFile("/hello.txt", "/foo.txt");

        char line[64];

        readFile("/foo.txt", 64, line);
        // strip newline
        char* pos = strchr(line, '\n');
        if (pos) {
            *pos = '\0';
        }
        ESP_LOGI(TAG, "Read from file: '%s'", line);

        // All done, unmount partition and disable SPIFFS
        disableSpiffs();
    }
*/
#endif