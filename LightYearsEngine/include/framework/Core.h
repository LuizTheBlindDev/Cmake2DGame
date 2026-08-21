#pragma once

#include <stdio.h>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace ly
{
	template<typename T>
	using unique = std::unique_ptr<T>;

	template<typename T>
	using shared = std::shared_ptr<T>;

	template<typename T>
	using weak = std::weak_ptr<T>;

	template<typename T>
	using List = std::vector<T>;

	template<typename keyType, typename vallType, typename Pr = std::less<keyType>>
	using Map = std::map<keyType, vallType, Pr>;

	template<typename keyType, typename vallType, typename hasher = std::hash<keyType>>
	using Dictionary = std::unordered_map<keyType, vallType, hasher>;

	template<typename T>
	using Set = std::unordered_set<T>;

	//using an alias to the char without confunding with char of "character" so we can use it instead of int to save memory
	using uint8 = unsigned char; 

	//Macros
	#define LOG(M, ...)	printf(M "\n", ##__VA_ARGS__)

	//box2d Macros
	#define B2_ID_EQUALS(id1,id2) ( id1.index1 == id2.index1 && id1.world0 == id2.world0 && id1.generation == id2.generation )

    #define B2_IS_NON_NULL(id) ( id.index1 != 0 )

	#define B2_IS_NULL(id) ( id.index1 == 0 )
}