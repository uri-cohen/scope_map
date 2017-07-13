// -*- mode: c++; -*-

// -----------------------------------------------------------------------------
// Scoped-Map
//
// Map object by key and and scope depth.
// That is, support a stack of mappings where inside scopes may see mappings
// of higher level (on "global" mappings) and can override them locally.

#include <string>
#include <map>
#include <stack>

template<typename ValueType=std::string, typename KeyType=std::string>
class ScopeMap
{
public:
    ScopeMap(const ValueType& defaultValue);
    virtual ~ScopeMap(void);

    void pushScope(void);
    void popScope(void);

    void set(const KeyType& key, const ValueType& value, bool bIsLocal=false);

    ValueType get(const KeyType& key);
    ValueType operator[](const KeyType& key) { return get(key); }

private:
    typedef std::map<KeyType, ValueType> MapType;

    typedef enum {
	SET,
	UNSET
    } ChangeOpType;
    
    struct ChangeOp {
	ChangeOpType _type;
	KeyType      _key;
	ValueType    _value;
    };

private:
    MapType                       _map;
    std::stack<ChangeOp>          _changeOps;
    std::stack<ChangeOp>          _postPushOps;
    std::stack<unsigned int>      _scopeSize;
    ValueType                     _defaultValue;
};

template<typename ValueType, typename KeyType>
ScopeMap<ValueType,KeyType>::ScopeMap(const ValueType& defaultValue)
    : _defaultValue(defaultValue)
{
    _scopeSize.push(0);
};

template<typename ValueType, typename KeyType>
ScopeMap<ValueType,KeyType>::~ScopeMap(void)
{
};

template<typename ValueType, typename KeyType>
void
ScopeMap<ValueType,KeyType>::pushScope(void)
{
}

template<typename ValueType, typename KeyType>
void
ScopeMap<ValueType,KeyType>::popScope(void)
{
}

template<typename ValueType, typename KeyType>
void
ScopeMap<ValueType,KeyType>::set(const KeyType& key, const ValueType& value, bool bIsLocal)
{
}

template<typename ValueType, typename KeyType>
ValueType
ScopeMap<ValueType,KeyType>::get(const KeyType& key)
{
    typename MapType::iterator it = _map.find(key);
    return (it == _map.end() ? (_map[key] = _defaultValue) : it->second);
}
