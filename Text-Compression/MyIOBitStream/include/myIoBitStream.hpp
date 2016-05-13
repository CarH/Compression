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
			bool internalEof;
			int bitsRead;
			void init();

		public:
			MyIOBitStream(std::iostream &s);
			MyIOBitStream();
			~MyIOBitStream();
			/**
			 * Append a bit at the end of the stream
			 * @param  b Boolean Indicating the bit to be appended. True for 1 and False for 0
			 * @return   Returns true if operation succeeded. False otherwise.
			 */
			bool appendBit(bool b);
			/**
			 * Append a byte at the end of the stream
			 * @param  byte Char representing the byte to be appended
			 * @return      REturn true if operation succeeded. Fase otherwise.
			 */
			bool appendByte(char byte);
			/**
			 * Reads the next bit of the stream, moving the pointer forwards.
			 * @return Returns the bit read. If EOF was reached, false is returned.
			 */
			bool getNextBit();
			/**
			 * REads the next byte from the stream, moving the pointer forwards.
			 * @return [description]
			 */
			char getNextByte();
			/**
			 * Get the number of bits read by the last call to getNextByte() . Designed to be used as a way of checking how many valid bits there are in return of last getNextByte call.
			 * @return Returns the value as specified.
			 */
			int getBitsReadCounter();
			/**
			 * Get the size of valid bits written to the internal char buffer.
			 * @return Returns the value as specified.
			 */
			int getBitsWriteCounter();
			/**
			 * Checks if the end of stream was reached. CAUTION: this method returns true only if a reading attemp has failed.
			 * @return Returns true if the last reading operation tried to read further than the end of stream.
			 */
			bool eof();
			//CUIDADO: o ponteiro de leitura da stream eh avancado ao chamar esse metodo
			//foi projetado para ser chamado somente no final
			std::string getString();
			/**
			 * Prints the bits stored in a string as a characters.
			 * @param str The string to be printed
			 */
			static void printBitString(std::string str);
	};
#endif 
