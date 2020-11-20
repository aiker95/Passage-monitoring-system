#pragma once


// Below after clasifaed template  will define a type of static array
// for next fixed length:
//		- for <4>  bytes type named ->  CodeVerify;
//		- for <8>  bytes type named ->	EightByteData;
//		- for <16> bytes type named ->	Data_16_Byte;
//		- for <24> bytes type named ->	Data_24_Byte.


struct IStaticByteArray
{
    virtual IStaticByteArray& operator = (const IStaticByteArray& rv) = 0;
    virtual unsigned char& operator [](unsigned int index) = 0;
    virtual const unsigned char& operator [](unsigned int index) const  = 0;
    virtual unsigned int Length() const = 0;
};


class ReferenceOnStaticArray : public IStaticByteArray
{
	public:
		ReferenceOnStaticArray(unsigned char* _p, unsigned int _size)
			: ref(_p)
			, length(_size)
		{}

		unsigned int Length() const
		{
			return length; 
		}
		
        IStaticByteArray& operator = (const IStaticByteArray& rv)
        {
            memcpy(ref, &rv[0], (length > rv.Length()) ? rv.Length() : length);
            return *this;
        }

		unsigned char& operator [](unsigned int index)
		{
			if (index > length)
				throw std::out_of_range("Max index");

			return ref[index];
		}

		const unsigned char& operator [](unsigned int index) const
		{
			if (index > length)
				throw std::out_of_range("Max index");

			return ref[index];
		}

	private:
		unsigned char*		ref;
		const unsigned int	length; 

		ReferenceOnStaticArray();
};


template <unsigned int _length>
struct StaticByteArray
{
    static const unsigned int length = _length; 
    typedef unsigned char TypeOfStaticArray[length];

    struct Reference : public IStaticByteArray
    {

        Reference(const Reference& v)
            : refBuf(v.refBuf)
        {
        }

        Reference(TypeOfStaticArray& v)
            : refBuf(&v)
        {
        } 


        Reference(StaticByteArray& v)
            : refBuf(&v.buf)
        {
        }
		
		
        Reference& operator = (const Reference& v)
        {
            refBuf = v.refBuf;
			return *this;
        }

        IStaticByteArray& operator = (const IStaticByteArray& rv)
        {
            memcpy(*refBuf, &rv[0], (_length > rv.Length()) ? rv.Length() : _length);
            return *this;
        }

        unsigned int Length() const
        {
            return StaticByteArray::length; 
        }

        unsigned char& operator [](unsigned int index)
        {
            if (index > length)
                throw std::out_of_range("Max index");

            return (*refBuf)[index];
        }

        const unsigned char& operator [](unsigned int index) const
        {
            if (index > length)
                throw std::out_of_range("Max index");

            return (*refBuf)[index];
        }

		template <int subLen>
		typename StaticByteArray<subLen>::TypeOfStaticArray& GetSubArray ()
		{
			return reinterpret_cast<StaticByteArray<subLen>::TypeOfStaticArray&>(*refBuf);
		}

		template <int subLen>
		typename const StaticByteArray<subLen>::TypeOfStaticArray& GetConstSubArray () const
		{
			return reinterpret_cast<StaticByteArray<subLen>::TypeOfStaticArray&>(*refBuf);
		}
		//template <int subLen>
  //      Reference(const StaticByteArray<subLen>::Reference& v)
  //          : refBuf(reinterpret_cast<TypeOfStaticArray&>(&v.buf[0]))
  //      {
  //      }

    private:
        TypeOfStaticArray (*refBuf);
    };


    //static const Reference& CreateConstReference(const TypeOfStaticArray& v)
    //{
    //  DoubleKey* kd = const_cast<DoubleKey*>(&KeyData);Data_16_Byte::Reference(*kd)
    //    return Reference( const_cast<TypeOfStaticArray>(v) );
    //}
    //-----

    TypeOfStaticArray buf;

    operator TypeOfStaticArray& ()
    {
        return buf;
    }
	
	
    template <int subSize>
	typename StaticByteArray<subSize>::TypeOfStaticArray& GetSubArrayRef ()
    {
        TypeOfStaticArray *refBuf = &buf;
		return reinterpret_cast<StaticByteArray<subSize>::TypeOfStaticArray&>(*refBuf);
	}

    template <int subSize>
	const typename StaticByteArray<subSize>::TypeOfStaticArray& GetSubArrayConstRef () const
    {
        const TypeOfStaticArray *refBuf = &buf;
		return reinterpret_cast<StaticByteArray<subSize>::TypeOfStaticArray&>(*refBuf);
	}

    Reference GetReference() const
    {
		Reference	ref((TypeOfStaticArray&)buf);
		return ref;
    }

    const Reference GetConstReference() const
    {
		const Reference	ref((TypeOfStaticArray&)buf);
		return ref;
    }
};

template <>
struct StaticByteArray<0>
{
	static const unsigned int length = 0;
	typedef unsigned char TypeOfStaticArray[1];
};

static const StaticByteArray<0>      emptyArray;

//typedef StaticByteArray<4>      FourByteData;
//typedef FourByteData			  CodeVerify;
//typedef StaticByteArray<8>      EightByteData;
//typedef StaticByteArray<16>     Data_16_Byte;
//typedef StaticByteArray<24>     Data_24_Byte;
