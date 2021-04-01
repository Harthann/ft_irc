#include <vector>
#include <string>
#include <algorithm>


namespace utils {

	std::vector<std::string>	split_params(std::string &datas) {
		std::vector<std::string>	ret;
		size_t						tmp = 0;
		size_t						tmp2 = 0;
		int							length;

		tmp = datas.find(' ', 0);
		do {
			tmp2 = datas.find(' ', tmp);
			if (tmp2 != std::string::npos)
				length = tmp2 - tmp;
			else
				length = datas.length() - tmp;
			ret.push_back(datas.substr(tmp, length));
		} while (tmp != std::string::npos);
		return ret;
	}
}