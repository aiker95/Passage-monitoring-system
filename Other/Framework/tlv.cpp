#include "stdafx.h"

#include "tlv.h"
#include <vector>
#include <algorithm>
#include "stuff.h"


//---------------------------------------------------------------------
//      -----------------  FOR TEST  ----------------------      !!!!!

void CompareStrings(std::string value, std::string etalon)
{
	if ( _stricmp(value.c_str(), etalon.c_str()) != 0)
    {
        throw std::exception(("\n<" + value + ">\n instead of\n<" + etalon + ">\n").c_str());
    }
}

bool Tlv::HimselfTest(std::string &reason)
{
	bool resault = false;

    unsigned char dump[] = { 
        0xe1, 0x5c, 
              0x9f, 0x1a, 0x02, 0x06, 0x43, 
              0x9f, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 
              0x5f, 0x2a, 0x02, 0x06, 0x43, 
              0x9a, 0x03, 0x11, 0x12, 0x05, 
              0x9c, 0x01, 0x00, 
              0x95, 0x05, 0x00, 0x00, 0x90, 0x00, 0x00, 
              0x9f, 0x37, 0x04, 0x43, 0x09, 0x35, 0x69, 
              0x82, 0x02, 0x58, 0x80, 
              0x9f, 0x26, 0x08, 0x8c, 0x13, 0x09, 0xc1, 0x9b, 0xa8, 0xbd, 0x5f, 
              0x9f, 0x27, 0x01, 0x80, 
              0x9f, 0x10, 0x12, 0x01, 0x10, 0xa0, 0x00, 0x03, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
              0x9f, 0x36, 0x02, 0x00, 0x38, 
              0x9f, 0x33, 0x03, 0x00, 0x08, 0xe8, 

        0x9f, 0x34, 0x03, 0x3f, 0x00, 0x01, 
        0x9f, 0x45, 0x02, 0x00, 0x00, 
        0x9f, 0x4c, 0x08, 0x00, 0x4d, 0x1e, 0x87, 0xd2, 0xf8, 0xff, 0xdc, 
        0x5a, 0x08, 0x54, 0x13, 0x33, 0x00, 0x57, 0x00, 0x40, 0x47, 
        0x5f, 0x34, 0x01, 0x00, 
        0x5f, 0x24, 0x03, 0x14, 0x12, 0x31, 
        0x50, 0x0a, 0x4d, 0x61, 0x73, 0x74, 0x65, 0x72, 0x43, 0x61, 0x72, 0x64, 
        0x57, 0x11, 0x54, 0x13, 0x33, 0x00, 0x57, 0x00, 0x40, 0x47, 0xd1, 0x41, 0x22, 0x01, 0x05, 0x54, 0x20, 0x97, 0x7f, 
        0x56, 0x00, 
        0x9b, 0x02, 0xc0, 0x00,
        0x5f, 0x20, 0x10, 0x43, 0x55, 0x53, 0x54, 0x20, 0x49, 0x4d, 0x50, 0x20, 0x4d, 0x43, 0x20, 0x34, 0x30, 0x34, 0x2f, 
        0x9f, 0x07, 0x02, 0xff, 0xc0, 
        0x9f, 0x0d, 0x05, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x9f, 0x0e, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x9f, 0x0f, 0x05, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x9f, 0x06, 0x07, 0xa0, 0x00, 0x00, 0x00, 0x04, 0x10, 0x10, 
        0x9f, 0x5d, 0x00, 
        0x9f, 0x74, 0x00,
        0x5f, 0x25, 0x03, 0x05, 0x01, 0x01, 
        0x9f, 0x34, 0x03, 0x3f, 0x00, 0x01, 
        0xff, 0xee, 0x01, 0x04,
        0xdf, 0x52, 0x01, 0x00, 
        0x84, 0x07, 0xa0, 0x00, 0x00, 0x00, 0x04, 0x10, 0x10
    };

    try
    {
        Tlv tlv(dump, sizeof(dump));
        CompareStrings(tlv.GetTagValue(0x00e1), "9f1a0206439f02060000000010005f2a0206439a031112059c0100950500009000009f370443093569820258809f26088c1309c19ba8bd5f9f2701809f10120110a00003240000000000000000000000ff9f360200389f33030008e8");

        CompareStrings(tlv.GetTagValue(0x5f2a), "0643");
        CompareStrings(tlv.GetTagValue(0x5f34), "00");
        CompareStrings(tlv.GetTagValue(0x5f24), "141231");
        CompareStrings(tlv.GetTagValue(0x5f20), "4355535420494d50204d43203430342f");
        CompareStrings(tlv.GetTagValue(0x5f25), "050101");

        CompareStrings(tlv.GetTagValue(0x9f1a), "0643");
        CompareStrings(tlv.GetTagValue(0x9f02), "000000001000");
        CompareStrings(tlv.GetTagValue(0x9f37), "43093569");
        CompareStrings(tlv.GetTagValue(0x9f26), "8c1309c19ba8bd5f");
        CompareStrings(tlv.GetTagValue(0x9f27), "80");
        CompareStrings(tlv.GetTagValue(0x9f10), "0110a00003240000000000000000000000ff");
        CompareStrings(tlv.GetTagValue(0x9f36), "0038");
        CompareStrings(tlv.GetTagValue(0x9f33), "0008e8");
        CompareStrings(tlv.GetTagValue(0x9f34), "3f0001");
        CompareStrings(tlv.GetTagValue(0x9f45), "0000");
        CompareStrings(tlv.GetTagValue(0x9f4c), "004d1e87d2f8ffdc");
        CompareStrings(tlv.GetTagValue(0x9f07), "ffc0");
        CompareStrings(tlv.GetTagValue(0x9f0d), "ffffffffff");
        CompareStrings(tlv.GetTagValue(0x9f0e), "0000000000");
        CompareStrings(tlv.GetTagValue(0x9f0f), "ffffffffff");
        CompareStrings(tlv.GetTagValue(0x9f06), "a0000000041010");
        CompareStrings(tlv.GetTagValue(0x9f5d), "");
        CompareStrings(tlv.GetTagValue(0x9f74), "");
        CompareStrings(tlv.GetTagValue(0x9f34), "3f0001");

        CompareStrings(tlv.GetTagValue(0xdf52), "00");

        CompareStrings(tlv.GetTagValue(0xffee), "04");

        CompareStrings(tlv.GetTagValue(0x50), "4d617374657243617264");
        CompareStrings(tlv.GetTagValue(0x56), "");
        CompareStrings(tlv.GetTagValue(0x57), "5413330057004047d1412201055420977f");
        CompareStrings(tlv.GetTagValue(0x5a), "5413330057004047");
        CompareStrings(tlv.GetTagValue(0x82), "5880");
        CompareStrings(tlv.GetTagValue(0x84), "a0000000041010");
        CompareStrings(tlv.GetTagValue(0x9a), "111205");
        CompareStrings(tlv.GetTagValue(0x9c), "00");
        CompareStrings(tlv.GetTagValue(0x95), "0000900000");
        CompareStrings(tlv.GetTagValue(0x9b), "c000");
	
		resault = true;
		reason = "All rigth!";
    }
    catch(const std::exception &e)
    {
        reason = e.what();
    }

	return resault;
}

// ^^^^^ -----------------  FOR TEST  ---------------------- ^^^^^^ !!!!!


TlvTagRef::TlvTagRef()
	: size(0), lenValue(0), value(NULL)
{
}

TlvTagRef::TlvTagRef(const unsigned char *dump)
	: size(0), lenValue(0), value(NULL)
{
    this->operator =(dump);
}


std::string TlvTagRef::InterpretSuchString()
{
	return std::string((char*)value, lenValue);
}


TlvTagRef& TlvTagRef::operator = (const unsigned char *dump)
{
	// Check is the simple length or multiple length.
	if (dump[0] <= 0x80)
	{
		value = dump + 1;
		lenValue = dump[0];
		size = lenValue + 1;
		return *this;
	}
	
	unsigned long lenLen = dump[0] - 0x80 + 1;
	unsigned long len = 0;
	for (unsigned long i = 1; i < lenLen; ++i)
	{
		if (len & 0xFF000000)
		{
			size = lenValue = 0;
			value = NULL;
			//throw std::exception("Incorrect length value!");
		}
		
		len = (len << 8) + dump[i];
	}

	size = len + lenLen;
	lenValue = len;
	value = dump + lenLen;
    
    return *this;
}


Tlv::Tlv(const unsigned char *dump, unsigned size, const char *ComplexTagNames)
{
    std::vector<TlvTagName> complexes;

    // build complex tag name list
    if (0 != ComplexTagNames)
    {
        const char hex[] = "0123456789ABCDEF0123456789abcdef";
        TlvTagName ComplexTagName = 0xf0000000;

        while (0 != *ComplexTagNames)
        {
            const char *pc = strchr(hex, *ComplexTagNames++);
            
            if (0 == pc)
            {
                complexes.push_back(ComplexTagName);
                ComplexTagName = 0xf0000000;
            }
            else
            {
                ComplexTagName = (ComplexTagName << 4) + ((pc - hex) & 0xf);
            }
        }

        if (0xf0000000 != ComplexTagName)
        {
            complexes.push_back(ComplexTagName);
        }
    }

    // parse tags
    TlvTagName tn = 0;
    for (unsigned i = 0; i < size-1; )
    {
        unsigned char x = dump[i++];
        tn = (tn << 8) | x;

        if (0x1f != (x & 0x1f))
        {
            TlvTagRef tr = (dump + i);

            if (tr.size > (size - i))
			{
				tr.size = 0;
				//Tags_.clear();
                //throw std::exception("Incorrect tag length!");
			}

            Tags_[tn] = tr;

            if (complexes.end() == std::find(complexes.begin(), complexes.end(), tn))
            {
                i += tr.size;
            }
            else
            {
                i += tr.size - tr.lenValue; // + lenLen
            }

			//LOG_TRACE << L"Found tag " << tn << END;
			
            tn = 0;
        }
    }
}

TlvTagValue Tlv::GetTagValue(TlvTagName tag) const
{
	TlvTagValue res;
    TlvTagRef tr = GetTagRef(tag);
	
    if (tr.lenValue > 0)
        res = stuff::ArrayToString((unsigned char*)tr.value, tr.lenValue);
	
    return res;
}

TlvTagRef Tlv::GetTagRef(TlvTagName tag) const
{
    TlvTags::const_iterator i = Tags_.find(tag);
    return Tags_.end() == i ? TlvTagRef() : i->second;
}

std::string Tlv::ToString() const
{
    std::string s;

    for each (const TlvTags::value_type &x in Tags_)
    {
        if (!s.empty())
        {
            s += "\n";
        }

        char TagName[10];
#if defined(_WIN32_WINNT) && (WINVER >= _WIN32_WINNT)
        sprintf_s(TagName, "%8x=", x.first);
#else
        sprintf(TagName, "%8x=", x.first);
#endif
        s += TagName + stuff::ArrayToString((unsigned char*)x.second.value, x.second.lenValue);
    }

    return s;
}


