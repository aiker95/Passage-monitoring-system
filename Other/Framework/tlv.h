/// @file
/// File contain the TLV parser class for work with TLV data buffer

#ifndef UNI_TLV_PARSER_H
#define UNI_TLV_PARSER_H

#include <string>
#include <map>


typedef unsigned long TlvTagName; // up to four bytes
typedef std::string TlvTagValue; // as hex string


/// @brief
/// Wrapper for one TLV record. It's check format length and save refer on data.
struct TlvTagRef 
{
	/// @brief
	/// Default constructor 
	TlvTagRef();
	
	/// @brief
	/// Constructor a set position of TLV record
	/// @param[in] Pointer on TLV data buffer
	TlvTagRef(const unsigned char *dump);
	
	/// @brief
	/// Set position of TLV record start
	/// @param[in] dump - pointer on start TLV data record 
	TlvTagRef& operator = (const unsigned char *dump);

	std::string InterpretSuchString();

	unsigned long 			size; // size of record (length+value)
	unsigned long 			lenValue;
	const unsigned char* 	value;
};


/// @brief
/// TLV data parser
class Tlv 
{
public:

	/// @brief
	/// This function to do testing himself and will return true if all right.
	/// It my be use for unit testing.
	static bool HimselfTest(std::string &reason = std::string());

	Tlv() {}

	/// @brief
	/// Constructor a parse binary dump of multiple TLV data records
	/// @param[in] dump - pointer on TLV data buffer
	/// @param[in] ComplexTagNames - array of tags name which consist  of several TLV records
    Tlv(const unsigned char *dump, unsigned size, const char *ComplexTagNames = "E1,77");
	
    virtual ~Tlv() {}

    TlvTagValue GetTagValue(TlvTagName tag) const;
	
    TlvTagRef GetTagRef(TlvTagName tag) const;

    std::string ToString() const;

	void swap(Tlv& _right)
	{
		Tags_.swap(_right.Tags_);
	}

private:
    typedef std::map<TlvTagName, TlvTagRef> TlvTags;
    TlvTags Tags_;

};

#endif  UNI_TLV_PARSER_H
