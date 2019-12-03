#include "matcher.h"
#include <iostream>
#include <algorithm>



bool ACMatcher::IsSep(uint8_t c) {
  return c == ' ' || c == ',' || c == '\0';
}

namespace {
uint8_t lower(uint8_t c) {
  if (c >= 0x41 && c <= 0x5A) {
    c |= 0x20;
  }
  return c;  
}
}

ACMatcher::~ACMatcher() {
  for (const auto& node: root_->branches) {
    DeleteNode(node.second);
  }
  delete root_;
  root_ = nullptr;
}

void ACMatcher::DeleteNode(Node* node) {
  for (const auto& branch: node->branches) {
    DeleteNode(branch.second);
  }
  delete node;  
}

ACMatcher::Node* ACMatcher::Next(const Node* node, uint8_t c) {
  for (const auto& item: node->branches) {
    if (item.first == c) {
      return item.second;
    }
  }
  return nullptr;
}

void ACMatcher::Build(const std::vector<std::string> keyword, int type) {
  BuildTrie(keyword);
  type_ = type;
  // BuildFailedJump();
}

void ACMatcher::BuildTrie(const std::vector<std::string> keywords) {
  auto root = new Node;
  root->c = 0;
  root->depth = 0;
  root->failed = root;
  root->parent = nullptr;
  root->teminated = false;  

  for (const auto& word: keywords) {
    Node* cur = root;
    for (auto c: word) {
      if (IsSep(c)) {
        continue;
      }
      c = lower(c);
      
      Node* next = Next(cur, c);      
      if (next == nullptr) {
        next = new Node;
        next->c = c;
        next->depth = cur->depth+1;
        next->failed = root;
        next->parent = cur;
        next->teminated = false;
        cur->branches.push_back(std::make_pair(c, next));
      }
      cur = next;
    }
    // insert word terminated flag 
    {
      cur->teminated = true;      
    }
  }
  root_ = root;
}

void ACMatcher::BuildFailedJump() {
  for (const auto& node: root_->branches) {
    BuildFailedJumpImpl(node.second);
  }  
}

void ACMatcher::BuildFailedJumpImpl(ACMatcher::Node* node) {
  Node* parent = node->parent;
  if (parent != root_) {
    auto failed = parent->failed;
    auto next = Next(failed, node->c);
    if (next == nullptr) {
      node->failed = root_;
    } else {
      node->failed = next;
    }
  }
  for (const auto& child: node->branches) {
    BuildFailedJumpImpl(child.second);
  }  
}

bool ACMatcher::Match(const std::string& sentence, std::string* pattern) {
  if (type_ == kHasWord) {
    return HasWord(sentence,pattern);
  }
  return ContainChars(sentence,pattern);

  // old match algorithm
  /*
  Node* cur = root_;
  size_t cur_index = 0;
  int first_pos = -1;
  
  for (const auto& c: sentence) {
    ++cur_index;
    if (IsSep(c)) {
      continue;
    }
    
    if (cur == root_) {
      first_pos = cur_index - 2;
    }      

    while (true) {
      Node* next = Next(cur, c);
      if (next == nullptr) {
        cur = cur->failed;
        if (cur == root_) {
          next = Next(cur, c);
          if (next != nullptr) {
            cur = next;
          }
          first_pos = cur_index - 2;
          break;
        } else if (cur->parent ==  root_) {
          first_pos = cur_index - 2;
          break;
        }
      } else {
        if (next->branches.empty()) {
          if (next->depth > 3 && (first_pos == -1 || IsSep(sentence[first_pos]))) {
            std::string reverse_pattern;            
            Node* reverse = next;
            while (reverse != root_) {
              reverse_pattern.push_back(reverse->c);
              reverse = reverse->parent;
            }
            std::reverse(reverse_pattern.begin(), reverse_pattern.end());
            *pattern = reverse_pattern;
            return true;
          } else {
            cur = root_;
            break;
          }
        }
        cur = next;
        break;
      }
    }
  }
  return false;
  */
}

bool ACMatcher::ReplaceWord(const std::string& sentence, std::string* pattern, std::vector<size_t> &vpos) {
  size_t i = 0;
  size_t last_pos = 0;
  auto len = sentence.size();

  while (i < len) {
    auto c = sentence[i];
    if (IsSep(c)) {
      ++i;
      last_pos = i;
      continue;
    }

    Node* cur = root_;
    while (i <= len) {
      c = lower(sentence[i]);
      if (IsSep(c)) {
        if (cur->teminated) {
          GetPattern(cur, pattern);
          vpos.push_back(last_pos);
          std::cout<<"last_pos: "<<last_pos << " word: " << sentence.substr (last_pos, i - last_pos+1)<<std::endl;
//          return true;
        }

        last_pos = i;
        break;
      }

      Node* next = Next(cur, c);
      if (next == nullptr) {
        while (!IsSep(sentence[i])) {
          ++i;
        }
        break;
      } else {
        cur = next;
        ++i;
      }
    }
  }
  return false;
}


bool ACMatcher::MatchAndReplace(const std::string& sentence, std::string* pattern, std::string &fill) {
  if (type_ == kHasWord) {
//	  size_t fill_len =
	std::vector<size_t> vpos;
	ReplaceWord(sentence, pattern, vpos);
//	for(auto pos : vpos){
//
//	}
  }
}

bool ACMatcher::HasWord(const std::string& sentence, std::string* pattern) {
  size_t i = 0;  
  auto len = sentence.size();
  
  while (i < len) {
    auto c = sentence[i];
    if (IsSep(c)) {
      ++i;
      continue;
    }
    
    Node* cur = root_;  
    while (i <= len) {
      c = lower(sentence[i]);
      if (IsSep(c)) {
        if (cur->teminated) {
          GetPattern(cur, pattern);
          return true;          
        }
        break;
      }

      Node* next = Next(cur, c);
      if (next == nullptr) {
        while (!IsSep(sentence[i])) {
          ++i;
        }
        break;
      } else {
        cur = next;
        ++i;
      }
    }
  }  
  return false;
}

void ACMatcher::GetPattern(const Node* cur, std::string* pattern) const {
  std::string reverse_pattern;
  const Node* reverse = cur;
  while (reverse != root_) {
    reverse_pattern.push_back(reverse->c);
    reverse = reverse->parent;
  }
  std::reverse(reverse_pattern.begin(), reverse_pattern.end());
  *pattern = reverse_pattern;
}

bool ACMatcher::ContainChars(const std::string& sentence, std::string* pattern) {
  size_t first_pos = 0;
  size_t i = 0;  
  auto len = sentence.size();
  
  while (i < len) {
    auto c = lower(sentence[i]);
    if (IsSep(c)) {
      ++i;
      continue;
    }    
    Node* cur = root_;
    first_pos = i;
    
    while (i <= len) {
      if (IsSep(sentence[i])) {
        if (cur->teminated) {
          GetPattern(cur, pattern);
          return true;          
        }
        break;
      }
      c = lower(sentence[i]);
      Node* next = Next(cur, c);
      if (next == nullptr) {
        if (cur->teminated) {
          GetPattern(cur, pattern);
          return true;          
        } else {
          i = ++first_pos;
          break;
        }
      } else {
        cur = next;
        ++i;
      }
    }
  }
  return false;
}


