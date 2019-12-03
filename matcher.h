#ifndef MICO_LIVE_MATCHER_H_
#define MICO_LIVE_MATCHER_H_

#include <vector>
#include <string>
#include <utility>

class ACMatcher {
 public:
  ~ACMatcher();
  enum {kHasWord, kContainChars};
  
  void Build(const std::vector<std::string> keyword, int type);
  bool Match(const std::string& sentence, std::string* pattern);
  bool MatchAndReplace(const std::string& sentence, std::string* pattern, std::string &fill);
 private:
  bool HasWord(const std::string& sentence, std::string* pattern);
  bool ContainChars(const std::string& sentence, std::string* pattern);

  void BuildTrie(const std::vector<std::string> keyword);
  void BuildFailedJump();
  bool IsSep(uint8_t c);
  bool ReplaceWord(const std::string& sentence, std::string* pattern, std::vector<size_t> &vpos);

  
  struct Node {
    uint8_t c;
    uint8_t depth;
    std::vector<std::pair<uint8_t, Node*>> branches;    
    Node* parent;
    Node* failed;
    bool teminated;
  };

  Node* Next(const Node* node, uint8_t c);
  void BuildFailedJumpImpl(Node* node);
  void DeleteNode(Node* node);
  
  void GetPattern(const Node* cur, std::string* pattern) const;
 private:
  Node* root_;
  int type_;
};

#endif // #define MICO_LIVE_MATCHER_H_ 
