#ifndef MAPSET_HPP
#define MAPSET_HPP

#include <map>
#include <set>
#include "unit.hpp"

template<class T, class S>
class Iterator : public std::iterator<std::forward_iterator_tag, std::pair<T,S>> {
    public:
		Iterator(typename std::map<T,std::set<S>>::iterator mIt, typename
				std::set<S>::iterator sIt) : mapIt(mIt), setIt(sIt) {

		}
        bool operator==(Iterator const& rhs) const {
			return this->mapIt == rhs.mapIt && this->setIt == rhs.setIt;
        }

        bool operator!=(Iterator const& rhs) const {
			return this->mapIt != rhs.mapIt || this->setIt != rhs.setIt;
        }

        Iterator& operator++() {
			return *this;
        }   

		std::pair<T,S> operator* () const {
			return std::pair<T,S>();
        }
    private:
		typename std::map<T,std::set<S>>::iterator mapIt;
		typename std::set<S>::iterator setIt;
};

template<typename T, typename S> class mapset {
public:
	// constructors
	mapset() { }
	mapset(const mapset<T,S>& source) {
		for(auto it : source.mapping) {
			for(auto jt : it.second) {
				this->insert(it.first, jt);
			}
		}
	}

	mapset(mapset<T,S>&& other) : mapping(other.mapping) {
	}

	~mapset() { }
	
	// assignment operator
	mapset& operator=(const mapset<T,S>& source);
	
	// basic set operations
	bool insert(T key, S value) {
		auto it = mapping.find(key);
		if(it != mapping.end()) { // mapping is present
			auto jt = it->second.find(value);
			if(jt != it->second.end()) { // value is present
				return false;
			} else { // value is not present
				it->second.insert(value);
				return true;
			}
		} else {
			mapping[key].insert(value);
			return true;
		}
	}

	bool erase(const T& key, const S& value) {
		auto it = mapping.find(key);
		if(it == mapping.end()) {
			return false;
		} else {
			auto jt = it->second.find(value);
			if(jt == it->second.end()) {
				return false;
			} else {
				it->second.erase(value);
				if(it->second.empty()) {
					mapping.erase(key);
				}
				return true;
			}
		}
	}

	std::set<S>& get(const T& key) {
		return mapping[key];
	}

	bool has(const T& key) const {
		return mapping.find(key) != mapping.end();
	}

	bool has(const T& key, const S& value) const {
		auto it = mapping.find(key);
		if(it == mapping.end()) {
			return false;
		} else {
			auto jt = it->second.find(value);
			if(jt == it->second.end()) {
				return false;
			} else {
				return true;
			}
		}
	}

	Iterator<T,S> begin() {
		auto it = this->mapping.begin();
		return Iterator<T,S>(it, it->second.begin());
	}

	Iterator<T,S> end() {
		auto it = this->mapping.rbegin();
		return Iterator<T,S>(this->mapping.end(), it->second.end());
	}

	bool empty() const {
		return mapping.empty();
	}
private:
	std::map<T,std::set<S>> mapping;
};

#ifdef UNITHPPPRESENT
UNITTEST(simpleConstructorTest) {
	mapset<int,float> ms1;
	AS_T(ms1.empty());
}

UNITTEST(simpleItTest) {
	mapset<int,float> ms1;
	auto it = ms1.begin();
	auto eit = ms1.end();
	AS_T(it == eit);
}

#endif
#endif
