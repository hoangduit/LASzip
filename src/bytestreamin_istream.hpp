/******************************************************************************
 *
 * Project:  integrating laszip into liblas - http://liblas.org -
 * Purpose:
 * Author:   Martin Isenburg
 *           isenburg at cs.unc.edu
 *
 ******************************************************************************
 * Copyright (c) 2010, Martin Isenburg
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Licence as published
 * by the Free Software Foundation.
 *
 * See the COPYING file for more information.
 *
 ****************************************************************************/

/*
===============================================================================

  FILE:  bytestreamin_istream.hpp
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    12 December 2010 -- created from ByteStreamOutFile after Howard got pushy (-;
  
===============================================================================
*/
#ifndef BYTE_STREAM_IN_ISTREAM_H
#define BYTE_STREAM_IN_ISTREAM_H

#include "bytestreamin.hpp"

#ifdef LZ_WIN32_VC6
#include <fstream.h>
#else
#include <istream>
#include <fstream>
using namespace std;
#endif

class ByteStreamInIstream : public ByteStreamIn
{
public:
  ByteStreamInIstream(istream& stream);
/* read a single byte                                        */
  unsigned int getByte();
/* read an array of bytes                                    */
  bool getBytes(unsigned char* bytes, unsigned int num_bytes);
/* is the stream seekable (e.g. standard in is not)          */
  bool isSeekable() const;
/* returns how many bytes were read since last reset         */
  unsigned int byteCount() const;
/* reset byte counter                                        */
  void resetCount();
/* destructor                                                */
  ~ByteStreamInIstream(){};
protected:
  istream& stream;
private:
#ifdef LZ_WIN32_VC6
  long start;
#else
  ios::off_type start;
#endif
};

class ByteStreamInIstreamLE : public ByteStreamInIstream
{
public:
  ByteStreamInIstreamLE(istream& stream);
/* read 16 bit low-endian field                              */
  bool get16bitsLE(unsigned char* bytes);
/* read 32 bit low-endian field                              */
  bool get32bitsLE(unsigned char* bytes);
/* read 64 bit low-endian field                              */
  bool get64bitsLE(unsigned char* bytes);
/* read 16 bit big-endian field                              */
  bool get16bitsBE(unsigned char* bytes);
/* read 32 bit big-endian field                              */
  bool get32bitsBE(unsigned char* bytes);
/* read 64 bit big-endian field                              */
  bool get64bitsBE(unsigned char* bytes);
private:
  unsigned char swapped[8];
};

class ByteStreamInIstreamBE : public ByteStreamInIstream
{
public:
  ByteStreamInIstreamBE(istream& stream);
/* read 16 bit low-endian field                              */
  bool get16bitsLE(unsigned char* bytes);
/* read 32 bit low-endian field                              */
  bool get32bitsLE(unsigned char* bytes);
/* read 64 bit low-endian field                              */
  bool get64bitsLE(unsigned char* bytes);
/* read 16 bit big-endian field                              */
  bool get16bitsBE(unsigned char* bytes);
/* read 32 bit big-endian field                              */
  bool get32bitsBE(unsigned char* bytes);
/* read 64 bit big-endian field                              */
  bool get64bitsBE(unsigned char* bytes);
private:
  unsigned char swapped[8];
};

inline ByteStreamInIstream::ByteStreamInIstream(istream& stream_param) :
  stream(stream_param)
{
  resetCount();
}

inline unsigned int ByteStreamInIstream::getByte()
{
  int byte = stream.get();
  if (stream.eof())
  {
//    fprintf(stderr, "reading EOF\n");
    byte = 0;
  }
  return (unsigned int)byte;
}

inline bool ByteStreamInIstream::getBytes(unsigned char* bytes, unsigned int num_bytes)
{
  stream.read((char*)bytes, num_bytes);
  return !!(stream.good());
}

inline bool ByteStreamInIstream::isSeekable() const
{
  return (!!(static_cast<ifstream&>(stream)));
}

inline unsigned int ByteStreamInIstream::byteCount() const
{
#ifdef LZ_WIN32_VC6
  return (stream.tellg() - start);
#else
  ios::pos_type end = stream.tellg();
  ios::off_type size = end - start;
  return static_cast<unsigned int>(size);
#endif
}

inline void ByteStreamInIstream::resetCount()
{
  start = stream.tellg();
}

inline ByteStreamInIstreamLE::ByteStreamInIstreamLE(istream& stream) : ByteStreamInIstream(stream)
{
}

inline bool ByteStreamInIstreamLE::get16bitsLE(unsigned char* bytes)
{
  return getBytes(bytes, 2);
}

inline bool ByteStreamInIstreamLE::get32bitsLE(unsigned char* bytes)
{
  return getBytes(bytes, 4);
}

inline bool ByteStreamInIstreamLE::get64bitsLE(unsigned char* bytes)
{
  return getBytes(bytes, 8);
}

inline bool ByteStreamInIstreamLE::get16bitsBE(unsigned char* bytes)
{
  if (getBytes(swapped, 2))
  {
    bytes[0] = swapped[1];
    bytes[1] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInIstreamLE::get32bitsBE(unsigned char* bytes)
{
  if (getBytes(swapped, 4))
  {
    bytes[0] = swapped[3];
    bytes[1] = swapped[2];
    bytes[2] = swapped[1];
    bytes[3] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInIstreamLE::get64bitsBE(unsigned char* bytes)
{
  if (getBytes(swapped, 8))
  {
    bytes[0] = swapped[7];
    bytes[1] = swapped[6];
    bytes[2] = swapped[5];
    bytes[3] = swapped[4];
    bytes[4] = swapped[3];
    bytes[5] = swapped[2];
    bytes[6] = swapped[1];
    bytes[7] = swapped[0];
    return true;
  }
  return false;
}

inline ByteStreamInIstreamBE::ByteStreamInIstreamBE(istream& stream) : ByteStreamInIstream(stream)
{
}

inline bool ByteStreamInIstreamBE::get16bitsLE(unsigned char* bytes)
{
  if (getBytes(swapped, 2))
  {
    bytes[0] = swapped[1];
    bytes[1] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInIstreamBE::get32bitsLE(unsigned char* bytes)
{
  if (getBytes(swapped, 4))
  {
    bytes[0] = swapped[3];
    bytes[1] = swapped[2];
    bytes[2] = swapped[1];
    bytes[3] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInIstreamBE::get64bitsLE(unsigned char* bytes)
{
  if (getBytes(swapped, 8))
  {
    bytes[0] = swapped[7];
    bytes[1] = swapped[6];
    bytes[2] = swapped[5];
    bytes[3] = swapped[4];
    bytes[4] = swapped[3];
    bytes[5] = swapped[2];
    bytes[6] = swapped[1];
    bytes[7] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInIstreamBE::get16bitsBE(unsigned char* bytes)
{
  return getBytes(bytes, 2);
}

inline bool ByteStreamInIstreamBE::get32bitsBE(unsigned char* bytes)
{
  return getBytes(bytes, 4);
}

inline bool ByteStreamInIstreamBE::get64bitsBE(unsigned char* bytes)
{
  return getBytes(bytes, 8);
}

#endif
