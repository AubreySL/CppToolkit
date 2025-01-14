#include <stdio.h>
#include <string.h>

struct file_header {
	int filesize;
	char filename[256];
};
#define ARRAY_NUM(arr) sizeof(arr)/sizeof(arr[0])

int pack()
{
	const char* file_arr[] = {
		"temp/2.lab","temp/2.mp3"
	};

	//create file
	FILE* fp;
	fopen_s(&fp, "temp/1.pack", "wb");
	if (NULL == fp)
	{
		printf("open new file failed !!!\n");
		return -1;
	}
	for (int i = 0; i < ARRAY_NUM(file_arr); ++i)
	{
		const char* filename = file_arr[i];
		//open file
		FILE* temp;
		fopen_s(&temp, filename, "rb");
		if (NULL == temp)
		{
			printf("open file: %s failed !!!\n", filename);
			continue;
		}
		//calculate file size
		fseek(temp, 0, SEEK_END);
		long filesize = ftell(temp);
		fseek(temp, 0, SEEK_SET);

		//write header
		struct file_header hdr = { 0 };
		hdr.filesize = filesize;
		strcpy_s(hdr.filename, filename);
		fwrite(&hdr, 1, sizeof(hdr), fp);

		//write file
		char buffer[4096] = { 0 };
		while (true)
		{
			int ret = fread(buffer, 1, 4096, temp);
			if (ret <= 0)break;
			fwrite(buffer, 1, ret, fp);
		}
		fclose(temp);

		printf("file: %s pack success. \n", filename);
	}
	//close
	fclose(fp);
	return 1;

}
void unpack()
{
	//todo
}
int main()
{
	pack();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
