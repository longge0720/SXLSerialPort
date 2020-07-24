#ifndef MYTOOLS_H
#define MYTOOLS_H

#include <QObject>

class MyTools
{
public:
    MyTools();
    static unsigned int calcIMXCrc(const char* filePath)
    {
        FILE *fp = fopen(filePath, "rb");
        if (NULL == fp)
        {
            printf("File:%s Not Found\n", filePath);
            return -1;
        }
        unsigned char pBuff='\0';

        unsigned int sum=0;

        int cur_length = 0;
        while((cur_length = fread(&pBuff, sizeof(unsigned char), sizeof(pBuff), fp)) > 0)
        {
            sum+=pBuff;
        }
        return sum;
    }
};

#endif // MYTOOLS_H
