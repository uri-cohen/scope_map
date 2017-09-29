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
#include <vector>
#include <exception>

#define SCOPE_MAP_EXCEPTION(_name)				         \
class _name##Exception : public ScopeMapException {                      \
public:									 \
    _name##Exception(const std::string& str) : ScopeMapException(str) {} \
    virtual ~_name##Exception(void) {}					 \
}

template<typename ValueType=std::string, typename KeyType=std::string>
class ScopeMap
{
    class ScopeMapException : public std::exception {
    public:
	ScopeMapException(const std::string& str) : _str(str) {}
	virtual ~ScopeMapException(void) {}
	virtual const char* what() const { return _str.c_str(); }
    private:
	std::string _str;
    };

    SCOPE_MAP_EXCEPTION(UnsupportedChangeOpType);
    
public:
    ScopeMap(const ValueType& defaultValue);
    virtual ~ScopeMap(void);

    void push(void);
    void pop(void);

    void set(const KeyType& key, const ValueType& value, bool bIsLocal=false);

    ValueType get(const KeyType& key);

private:
    typedef std::map<KeyType, ValueType> MapType;

    typedef enum {
	NOP,
	SET,
	UNSET,
	POST_PUSH_SET,
	POST_PUSH_UNSET,
    } ChangeOpType;
    
    struct ChangeOp {
	ChangeOp(void) : _type(NOP) {}
	ChangeOp(ChangeOpType type, const KeyType& key, const ValueType& value)
	    : _type(type), _key(key), _value(value) {}
	ChangeOp(const ChangeOp& other)
	    : _type(other._type), _key(other._key), _value(other._value) {}

	ChangeOpType _type;
	KeyType      _key;
	ValueType    _value;
    };

private:
    MapType                       _map;
    std::stack<ChangeOp>          _changeOps;
    std::vector<ChangeOp>         _postPushOps;
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
ScopeMap<ValueType,KeyType>::push(void)
{
}

// restore the previos state...
// that is, unrole in reverse order the change log to the last push point.
// special care for handling "local" scope mappings
template<typename ValueType, typename KeyType>
void
ScopeMap<ValueType,KeyType>::pop(void)
{
    // no more push are expected from this scope. clear the post push stack
    _postPushOps.erase(_postPushOps.begin(), _postPushOps.end());

    for(int nRoleBack = _scopeSize.top(); nRoleBack; --nRoleBack) {
	ChangeOp& op  = _changeOps.top();
	switch (op._type) {
	case SET:
	    _map[op._key] = op._value;
	    break;
	case UNSET:
	    _map.erase(op._key);
	    break;
	case POST_PUSH_SET:
	    _postPushOps.push_back(ChangeOp(SET, op._key, op._value));
	    break;
	case POST_PUSH_UNSET:
	    _postPushOps.push_back(ChangeOp(UNSET, op._key, op._value));
	    break;
	default:
	    throw UnsupportedChangeOpTypeException(
		"pop: Unsupported ChangeOp type: " + std::to_string(op._type));
	}
    }

    _scopeSize.pop();
}

template<typename ValueType, typename KeyType>
void
ScopeMap<ValueType,KeyType>::set(const KeyType& key,
				 const ValueType& value,
				 bool bIsLocal)
{
    typename MapType::iterator it = _map.find(key);
    if (it == _map.end()) {
	_changeOps.push(ChangeOp(UNSET, key, value));
	if (bIsLocal) {
	    _postPushOps.push_back(ChangeOp(UNSET, key, value));
	}
    } else {
	_changeOps.push(ChangeOp(SET, key, it->second));
	if (bIsLocal) {
	    _postPushOps.push_back(ChangeOp(SET, key, it->second));
	}
    }
    it->second = value;
    _scopeSize.top() += 1;
}

template<typename ValueType, typename KeyType>
ValueType
ScopeMap<ValueType,KeyType>::get(const KeyType& key)
{
    typename MapType::iterator it = _map.find(key);
    return (it == _map.end() ? (_map[key] = _defaultValue) : it->second);
}
