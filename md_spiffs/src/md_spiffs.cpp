/* SPIFFS filesystem
   This code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

// TODO append(), peek(), convert() ...
#include <Arduino.h>
#include <md_spiffs.h>
//#include <stat.h>

/*
  #include <stdio.h>
  #include <string.h>
  #include <sys/unistd.h>
  #include <sys/stat.h>
  #include <stdlib.h>
  #include "esp_err.h"
  #include "esp_log.h"
  #include "esp_spiffs.h"
  #define FFS_MAXLEN 50
*/
md_spiffs         *_pffs     = NULL;
static const char *TAG       = "SPIFFS";
static const char *base_path = "/spiffs";
char buffer[FFS_MAXLEN];

// --- class md_spiffs

esp_err_t md_spiffs::init(md_spiffs* pffs, bool    formatOnFail, const char * basePath,
                                           uint8_t maxOpenFiles, const char * partitionLabel)
  {
    _pffs  = pffs;
    _total = _pffs->totalBytes();
    _used  = _pffs->usedBytes();
    this->begin(formatOnFail, basePath, maxOpenFiles, partitionLabel);
    SOUT(" SPIFFS started used/total bytes "); SOUTHEX(_used);
    SOUT("/"); SOUTHEX(_total); SOUT("  ");
    SOUT(_used); SOUT("/"); SOUTLN(_total);
  }

void      md_spiffs::clearBuffer()
  {
    memset(buffer, 0, FFS_MAXLEN);
  }

uint8_t   md_spiffs::writeFile  (char* filename, const char* text)
  {
    if(filename[0] != '/')
      {
        SOUT(TAG); SOUTLN(" Filename must begin with '/");
        return 1;
      }
    clearBuffer();
    strcpy(buffer, base_path);
    strcat(buffer, (const char*) filename);
    FILE* f = fopen(buffer, "w");
    if (f == NULL)
      {
        SOUT(TAG); SOUTLN(" Failed to open file for writing");
        return 1;
      }
    fprintf(f, text);
    fclose(f);
    SOUT(TAG); SOUT(" File writen '"); SOUT(buffer); SOUTLN("'");
    return ESP_OK;
  }

uint8_t   md_spiffs::deleteFile (const char* filename)
 {
    esp_err_t ret = ESP_OK;

    if(filename[0] != '/')
      {
        ESP_LOGE(TAG, "filename must begin with '/");
        return 1;
      }
    clearBuffer();
    strcpy(buffer, base_path);
    strcat(buffer, filename);
    // Check if destination file exists before renaming
          // struct stat st;
          // if (stat(buffer, &st) == 0)
    if (this->exists(buffer))
      {
        // Delete it if it exists
        unlink(buffer);
        ESP_LOGI(TAG, "file deleted: %s", filename);
        ret = ESP_OK;
      }
      else
      {
        ESP_LOGE(TAG, "file don't exist: %s", filename);
        ret = 1;
      }

    return ret;
  }


uint8_t   md_spiffs::renameFile (const char* filename_src, const char* filename_tar)
  {
    esp_err_t ret = ESP_OK;

    if(filename_src[0] != '/' || filename_tar[0] != '/')
      {
        ESP_LOGE(TAG, "Filename must begin with '/");
        return 1;
      }
    clearBuffer();

    // Check if file exists
    strcpy(buffer, base_path);
    strcat(buffer, filename_src);
          // struct stat st;
          // if (stat(buffer, &st) == 0)
    if (this->exists(buffer))
      {
        uint8_t memSize = strlen(base_path) + strlen(filename_tar);
        char* tarBuf = (char*) malloc(memSize*sizeof(char));
        strcpy(tarBuf, base_path);
        strcat(tarBuf,filename_tar);

        if (rename(buffer, tarBuf) != 0)
          {
            ESP_LOGE(TAG, "failed to rename file: %s -> %s", buffer, tarBuf);
            ret = 1;
          }
          else
          {
            ESP_LOGI(TAG, "file renamed: %s -> %s", buffer, tarBuf);
            ret = ESP_OK;
          }
        free(tarBuf);
        return ret;
      }
      else
      {
        ESP_LOGE(TAG, "file don't exist: %s", filename_src);
        return 1;
      }
  }

uint8_t   md_spiffs::readFile   (const char* filename, uint16_t len, char* textBuf)
  {
    if(filename[0] != '/')
      {
        ESP_LOGE(TAG, "Filename must begin with '/");
        return 1;
      }
    clearBuffer();
    strcpy(buffer, base_path);
    strcat(buffer, (const char*) filename);
    FILE* f = fopen(buffer, "r");
    if (f == NULL)
      {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return 1;
      }
    fgets(textBuf, len, f);
    fclose(f);
    ESP_LOGI(TAG, "Read %d bytes from file: %s", len, buffer);
    return ESP_OK;
  }

  /*
    esp_err_t md_spiffs::init(bool    formatOnFail, const char * basePath,
                              uint8_t maxOpenFiles, const char * partitionLabel)
      {
        ESP_LOGI(TAG, "Initializing SPIFFS");
        esp_err_t ret = esp_vfs_spiffs_register(conf);

        if (ret != ESP_OK)
          {
            if (ret == ESP_FAIL)
              { ESP_LOGE(TAG, "Failed to mount or format filesystem"); }
              else if (ret == ESP_ERR_NOT_FOUND)
                { ESP_LOGE(TAG, "Failed to find SPIFFS partition"); }
              else
                { ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret)); }
            return 1;
          }
        size_t total = 0, used = 0;
        ret = esp_spiffs_info(NULL, &total, &used);
        if (ret != ESP_OK)
          {
            ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
          }
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
        return ESP_OK;
      }
  */
/*
  uint8_t disableSpiffs()
    {
      esp_vfs_spiffs_unregister(NULL);
      ESP_LOGI(TAG, "SPIFFS unmounted");
      return ESP_OK;
    }
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