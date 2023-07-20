

void usart_init(int usart_port, int baud_rate, int rx_int_en);
void usart_1_tx(char  data);
void usart_2_tx(char  data);
void usart_3_tx(char  data);

void serialprint_1(char * data_string);
void serialprint_2(char * data_string);
void serialprint_3(char * data_string);

void serialprintln_1(char * data_string);
void serialprintln_2(char * data_string);
void serialprintln_3(char * data_string);

char usart_1_rx();
char usart_2_rx();
char usart_3_rx();

