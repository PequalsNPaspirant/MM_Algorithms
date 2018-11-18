#include <unordered_map>
#include <string>
using namespace std;

#include "Trie/Trie_implementationUsingArray.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	int globalCounter_implementationUsingArray = 0;
	const int CHILDREN_SIZE = 26;

	struct TrieNode_implementationUsingArray
	{
		TrieNode_implementationUsingArray()
			: isLeaf(false)
		{
			for (int i = 0; i < CHILDREN_SIZE; ++i)
				children[i] = nullptr;

			++globalCounter_implementationUsingArray;
		}

		~TrieNode_implementationUsingArray()
		{
			--globalCounter_implementationUsingArray;
		}

		bool isLeaf;
		int childrenCount;
		TrieNode_implementationUsingArray* children[CHILDREN_SIZE]; // only words with small letters can be stored
	};

	// Trie can be represented as a global head pointer or a class having head pointer
	//TrieNode* head = nullptr;

	class Trie_implementationUsingArray
	{
	public:
		Trie_implementationUsingArray()
			: head_(new TrieNode_implementationUsingArray())
		{
		}

		void deleteNode(TrieNode_implementationUsingArray* node)
		{
			if (node == nullptr)
				return;

			for (int i = 0; i < CHILDREN_SIZE; ++i)
				deleteNode(node->children[i]);

			delete node;
		}

		~Trie_implementationUsingArray()
		{
			deleteNode(head_);
		}

		static int getTotalChildren(TrieNode_implementationUsingArray* node)
		{
			if (node == nullptr)
				return 0;

			int counter = 0;
			for (int i = 0; i < CHILDREN_SIZE; ++i)
			{
				if (node->children[i] != nullptr)
					++counter;
			}

			return counter;
		}

		bool insert(const string& str)
		{
			TrieNode_implementationUsingArray* current = head_;
			for (int i = 0; i < str.length(); ++i)
			{
				if (current->children[str[i] - 'a'] == nullptr)
					current->children[str[i] - 'a'] = new TrieNode_implementationUsingArray();

				current = current->children[str[i] - 'a'];
			}

			current->isLeaf = true;
			return true;
		}

		bool find(const string& str)
		{
			TrieNode_implementationUsingArray* current = head_;
			for (int i = 0; i < str.length(); ++i)
			{
				if (current == nullptr || current->children[str[i] - 'a'] == nullptr)
					return false;

				current = current->children[str[i] - 'a'];
			}

			return current->isLeaf;
		}

		bool erase(const string& str)
		{
			TrieNode_implementationUsingArray* startTrieNode = head_;
			int startIndex = -1;
			TrieNode_implementationUsingArray* current = head_;
			for (int i = 0; i < str.length(); ++i)
			{
				if (current == nullptr || current->children[str[i] - 'a'] == nullptr)
					return false;

				current = current->children[str[i] - 'a'];

				if (i < str.length() - 1 && (current->isLeaf || getTotalChildren(current) > 1) ) // current node is leaf of some other key OR it has multiple children
				{
					startTrieNode = current;
					startIndex = i;
				}
			}

			current->isLeaf = false; // Key exists in Trie. Mark it as false which means its not present in Trie anymore.
			 
			if (getTotalChildren(current) == 0) // Only if there are no children after this node
			{
				current = startTrieNode->children[str[++startIndex] - 'a'];
				startTrieNode->children[str[startIndex] - 'a'] = nullptr;
				for (; ++startIndex < str.length(); )
				{
					TrieNode_implementationUsingArray* temp = current->children[str[startIndex] - 'a'];
					delete current;
					current = temp;
				}
				delete current;
			}

			return true;
		}

	private:
		TrieNode_implementationUsingArray * head_;
	};


	// Test
	MM_DECLARE_FLAG(Trie_implementationUsingArray);

	MM_UNIT_TEST(Trie_implementationUsingArray_test_1, Trie_implementationUsingArray)
	{
		vector<string> data{
			"helloworld",
			"hello",
			"h",
			"helll",
			"hell",
			"hellow",
			"code", 
			"coder", 
			"coding", 
			"codable",
			"codec", 
			"codecs", 
			"coded"
			"codeless", "codependence", "codependency", "codependent",
			"codependents", "codes", "codesign", "codesigned", "codeveloped",
			"codeveloper", "codex", "codify", "codiscovered", "codrive"
		};

		{
			Trie_implementationUsingArray t;
			for (int i = 0; i < data.size(); ++i)
				t.insert(data[i]);

			for (int i = 0; i < data.size(); ++i)
				t.erase(data[i]);
		}
		MM_EXPECT_TRUE(globalCounter_implementationUsingArray == 0, globalCounter_implementationUsingArray);

		{
			Trie_implementationUsingArray t;
			for (int i = 0; i < data.size(); ++i)
				t.insert(data[i]);

			for (int i = data.size() - 1; i > 0; --i)
				t.erase(data[i]);
		}
		MM_EXPECT_TRUE(globalCounter_implementationUsingArray == 0, globalCounter_implementationUsingArray);
	}

}