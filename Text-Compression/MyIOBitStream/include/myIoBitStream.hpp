#include <sstream>
#include <string>
#include <sstream>
#include <iostream>
#ifndef __MY_IO_BITSTREAM__
	#define __MY_IO_BITSTREAM__
	class MyIOBitStream{
		private:
			char charBuff;
			short int bitCounterRead;
			short int bitCounterWrite;
			std::iostream *stream;
			bool wasStreamDeclaredInternally;

		public:
			MyIOBitStream(std::iostream &s);
			MyIOBitStream();
			~MyIOBitStream();
			bool appendBit(bool b);
			bool appendByte(char byte);
			bool getNextBit();
			char getNextByte();
			//CUIDADO: o ponteiro de leitura da stream eh avancado ao chamar esse metodo
			//foi projetado para ser chamado somente no final
			std::string getString();
			static void printBitString(std::string str);
	};
#endif 
