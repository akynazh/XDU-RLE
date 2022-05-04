#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void Solve(char* choose, char* inputFileName, char* outputFileName){
	FILE* inputFile = fopen(inputFileName, "rb");
	FILE* outputFile = fopen(outputFileName, "wb");
	if(inputFile == NULL || outputFile == NULL){
		if(inputFile != NULL)	fclose(inputFile);
		if(outputFile != NULL)	fclose(outputFile); 
		printf("can't find the file");
		return;
	}
	unsigned char* inputByte = (unsigned char*)malloc(sizeof(unsigned char)*1024*1024*1024);
	unsigned char* outputByte = (unsigned char*)malloc(sizeof(unsigned char)*1024*1024*1024);
	if(!strcmp(choose, "-c")){
		while(fread(inputByte, sizeof(unsigned char), 1024, inputFile) != 0){
			int size = Encode(inputByte, outputByte);
			fwrite(outputByte, sizeof(unsigned char), size, outputFile);
		} 
	}else{
		while(fread(inputByte, sizeof(unsigned char), 1024, inputFile) != 0){
			int size = Decode(inputByte, outputByte);
			fwrite(outputByte, sizeof(unsigned char), size, outputFile);
		}
	}
	printf("success"); 
	fclose(inputFile);	
	fclose(outputFile);
}
int Encode(unsigned char* inputByte, unsigned char* outputByte){
	unsigned char *in = inputByte, *out = outputByte;
	int left = strlen(inputByte), size = 0;
	while(left > 0){
		if(left < 3){
			out[size++] = *in++;
			left--;
		}else if(in[0] == in[1] && in[1] == in[2]){
			int i = 0, len = 1; 
			while(i < left - 1 && len < 127){//�����ư�λ��ʾ��Χ��-128��127�������λΪ127 
				if(in[i] == in[i+1]){
					len++;	i++;
				}else	break;
			}
			out[size++] = len|0x80;//��0x80��������10000000��λ�� ȡ����λ���ѵڰ�λ��Ϊһ ��Ϊѹ����� 
			out[size++] = *in;
			in += len;	
			left -= len;
		}else{
			out[size++] = *in++;	
			left--;
		}
	}
	return size;
}
int Decode(unsigned char* inputByte, unsigned char* outputByte){
	unsigned char *in = inputByte, *out = outputByte;
	int i = 0, size = 0;
	while(i < strlen(inputByte)){
		//���ﰴλ���������������� ����ͱ����in[i]&(0x80==0x80)��Ϊ���ȼ�==����&
		if((in[i] & 0x80) == 0x80){//�����ѹ����� ����0x80��λ�������0x80
			int c = in[i] & 0x7f;//ȡ�º���λ�� ���ظ��ֽڵ�����
			i++;
			while(c--)	out[size++] = in[i];
			i++;
		}else{
			out[size++] = in[i++];
		}
	}
	return size;
}
int main(int argc, char *argv[]){
	if(!strcmp(argv[2], "-c") || !strcmp(argv[2], "-d"))	Solve(argv[2], argv[1], argv[3]);
	else	printf("illegal input");
	return 0;
} 
