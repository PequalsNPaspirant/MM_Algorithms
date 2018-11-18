#include <unordered_map>
#include <string>
using namespace std;

#include "Trie/Trie_implementationUsingHashMap.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	int globalCounter = 0;

	struct TrieNode_implementationUsingHashMap
	{
		TrieNode_implementationUsingHashMap()
			: isLeaf(false)
		{
			++globalCounter;
		}

		~TrieNode_implementationUsingHashMap()
		{
			--globalCounter;
		}

		bool isLeaf;
		unordered_map<char, TrieNode_implementationUsingHashMap*> children;
	};

	// Trie can be represented as a global head pointer or a class having head pointer
	//TrieNode* head = nullptr;

	class Trie_implementationUsingHashMap
	{
	public:
		Trie_implementationUsingHashMap()
			: head_(new TrieNode_implementationUsingHashMap())
		{
		}

		void deleteNode(TrieNode_implementationUsingHashMap* node)
		{
			if (node == nullptr)
				return;

			for (unordered_map<char, TrieNode_implementationUsingHashMap*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
				deleteNode(it->second);

			delete node;
		}

		~Trie_implementationUsingHashMap()
		{
			deleteNode(head_);
		}

		bool insert(const string& str)
		{
			TrieNode_implementationUsingHashMap* current = head_;
			for (int i = 0; i < str.length(); ++i)
			{
				if (current->children.find(str[i]) == current->children.end())
					current->children[str[i]] = new TrieNode_implementationUsingHashMap();

				current = current->children[str[i]];
			}

			current->isLeaf = true;
			return true;
		}

		bool find(const string& str)
		{
			TrieNode_implementationUsingHashMap* current = head_;
			for (int i = 0; i < str.length(); ++i)
			{
				if (current == nullptr || current->children.find(str[i]) == current->children.end())
					return false;

				current = current->children[str[i]];
			}

			return current->isLeaf;
		}

		bool erase(const string& str)
		{
			TrieNode_implementationUsingHashMap* startTrieNode = head_;
			int startIndex = -1;
			TrieNode_implementationUsingHashMap* current = head_;
			for (int i = 0; i < str.length(); ++i)
			{
				if (current == nullptr || current->children.find(str[i]) == current->children.end())
					return false;

				current = current->children[str[i]];

				if (i < str.length() - 1 && (current->isLeaf || current->children.size() > 1) ) // current node is leaf of some other key OR it has multiple children
				{
					startTrieNode = current;
					startIndex = i;
				}
			}

			current->isLeaf = false; // Key exists in Trie. Mark it as false which means its not present in Trie anymore.
			 
			if (current->children.size() == 0) // Only if there are no children after this node
			{
				current = startTrieNode->children[str[++startIndex]];
				startTrieNode->children[str[startIndex]] = nullptr;
				for (; ++startIndex < str.length(); )
				{
					TrieNode_implementationUsingHashMap* temp = current->children[str[startIndex]];
					delete current;
					current = temp;
				}
				delete current;
			}

			return true;
		}

	private:
		TrieNode_implementationUsingHashMap * head_;
	};


	// Test
	MM_DECLARE_FLAG(Trie_implementationUsingHashMap);

	MM_UNIT_TEST(Trie_implementationUsingHashMap_test_1, Trie_implementationUsingHashMap)
	{
		vector<string> data {
			"helloworld",
			"hello",
			"h",
			"helll",
			"hell",
			"hellow",
			"code", "coder", "coding", "codable", "codec", "codecs", "coded",
			"codeless", "codependence", "codependency", "codependent",
			"codependents", "codes", "codesign", "codesigned", "codeveloped",
			"codeveloper", "codex", "codify", "codiscovered", "codrive"
		};

		{
			Trie_implementationUsingHashMap t;
			for (int i = 0; i < data.size(); ++i)
				t.insert(data[i]);

			for (int i = 0; i < data.size(); ++i)
				t.erase(data[i]);
		}
		MM_EXPECT_TRUE(globalCounter == 0, globalCounter);

		{
			Trie_implementationUsingHashMap t;
			for (int i = 0; i < data.size(); ++i)
				t.insert(data[i]);

			for (int i = data.size() - 1; i >= 0; --i)
				t.erase(data[i]);
		}
		MM_EXPECT_TRUE(globalCounter == 0, globalCounter);
	}

}