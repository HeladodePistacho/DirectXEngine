#include <exception>
#include <string>

class custom_exception : public std::exception
{
public:
	custom_exception(const char* _title, const char* _content) : title(_title), content(_content) {};

	void Fill(const char* new_title, const char* new_content)
	{
		title = new_title;
		content = new_content;
	}
	const char* GetTitle() const { return title.c_str(); }
	const char* GetContent() const { return content.c_str(); }
private: 
	std::string title, content;
};