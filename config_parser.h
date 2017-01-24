// An nginx config file parser.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

const int default_port_number = 80;

class NginxConfig;

// The parsed representation of a single config statement.
class NginxConfigStatement {
 public:
  std::string ToString(int depth);
  std::vector<std::string> tokens_;
  std::unique_ptr<NginxConfig> child_block_;
};

// The parsed representation of the entire config.
class NginxConfig {
 public:
  std::string ToString(int depth = 0);
  std::vector<std::shared_ptr<NginxConfigStatement>> statements_;
};

// The driver that parses a config file and generates an NginxConfig.
class NginxConfigParser {
 public:
  NginxConfigParser() {}

  // Take a opened config file or file name (respectively) and store the
  // parsed config in the provided NginxConfig out-param.  Returns true
  // iff the input config file is valid.
  bool Parse(std::istream* config_file, NginxConfig* config);
  bool Parse(const char* file_name, NginxConfig* config);

 private:
  enum TokenType {
    TOKEN_TYPE_START = 0,
    TOKEN_TYPE_NORMAL = 1,
    TOKEN_TYPE_START_BLOCK = 2,
    TOKEN_TYPE_END_BLOCK = 3,
    TOKEN_TYPE_COMMENT = 4,
    TOKEN_TYPE_STATEMENT_END = 5,
    TOKEN_TYPE_EOF = 6,
    TOKEN_TYPE_ERROR = 7
  };
  const char* TokenTypeAsString(TokenType type);

  enum TokenParserState {
    TOKEN_STATE_INITIAL_WHITESPACE = 0,
    TOKEN_STATE_SINGLE_QUOTE = 1,
    TOKEN_STATE_DOUBLE_QUOTE = 2,
    TOKEN_STATE_TOKEN_TYPE_COMMENT = 3,
    TOKEN_STATE_TOKEN_TYPE_NORMAL = 4
  };

  TokenType ParseToken(std::istream* input, std::string* value);
};

class GetPortNumber {
public:
    GetPortNumber(NginxConfig config)
    {
        port_number = getPortNumber(config);
    }
    int portNumber()
    {
        return port_number;
    }
private:
    int port_number;
    int getPortNumber(NginxConfig config)
    {
        for (const auto& statement : config.statements_)
        {
            if (statement->tokens_[0] == "server" &&  statement->child_block_ != nullptr)
            {
                for (const auto& stuff : statement->child_block_->statements_)
                {
                    // std::cout << stuff->tokens_[0] << "\n" ;
                    if (stuff->tokens_[0] == "listen")
                    {
                        if (stuff->tokens_.size() != 2)
                        {
                            return default_port_number;
                        }
                        std::string port_string = stuff->tokens_[1];
                        
                        for (int i = 0; i < port_string.length(); i++) {
                            if (!isdigit(port_string[i]))
                            {
                                return default_port_number;
                            }
                        }
                        
                        int port_num = std::stoi(port_string);
                        return port_num;
                    }
                }
            }
        }
        return default_port_number;
    }
};




