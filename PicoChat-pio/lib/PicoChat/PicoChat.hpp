#include <Arduino.h>

struct message_packet
{
public:
  uint8_t id; // 0 - ack message
  uint8_t user_name_length;
  uint8_t message_length;
  char *user_name;
  char *message;

  message_packet(uint8_t id, const char *usr_name, const char *text)
  {
    this->id = id;
    this->user_name_length = strlen(usr_name);
    this->message_length = strlen(text);
    user_name = (char *)malloc(user_name_length + 1);
    message = (char *)malloc(message_length + 1);
    // memcpy(user_name, usr_name, user_name_length);
    // memcpy(message, text, message_length);
    strcpy(user_name, usr_name);
    strcpy(message, text);
  }

  message_packet(uint8_t *buf)
  {
    this->id = buf[0];
    this->user_name_length = buf[1];
    this->message_length = buf[2];
    user_name = (char *)malloc(user_name_length + 1);
    message = (char *)malloc(message_length + 1);
    // memcpy(user_name, &buf[3], user_name_length);
    // memcpy(message, &buf[3 + user_name_length], message_length);
    memcpy(user_name, &buf[3], user_name_length);
    memcpy(message, &buf[3 + user_name_length], message_length);
    user_name[user_name_length] = '\0';
    message[message_length] = '\0';
  }

  uint16_t getPacketSize() const
  {
    return 3 + user_name_length + message_length;
  }

  // Funkcja serializująca strukturę do tablicy bajtów
  uint8_t *toByteArray() const
  {
    uint16_t total_size = getPacketSize();
    uint8_t *buffer = (uint8_t *)malloc(total_size);

    if (buffer != NULL)
    {
      // Zapisz nagłówek
      buffer[0] = id;
      buffer[1] = user_name_length;
      buffer[2] = message_length;
      memcpy(&buffer[3], user_name, user_name_length);
      memcpy(&buffer[3 + user_name_length], message, message_length);
    }
    return buffer;
  }

  ~message_packet()
  {
    free(user_name);
    free(message);
  }
};