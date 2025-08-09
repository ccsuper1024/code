#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h"

#define MAX_DESC_LEN     256
#define MAX_ERROR_COUNT  1024

typedef struct {
    int code;
    char desc[MAX_DESC_LEN];
} ErrorEntry;

typedef struct {
    ErrorEntry list[MAX_ERROR_COUNT];
    int count;
} ErrorTable;

// 排序函数：按错误码升序
int compare_error_entry(const void *a, const void *b) {
    return ((ErrorEntry *)a)->code - ((ErrorEntry *)b)->code;
}

// 读取 JSON 文件并填充 ErrorTable
int load_error_codes(const char *filename, ErrorTable *table) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);

    char *data = (char *)malloc(len + 1);
    if (!data) {
        fclose(fp);
        return -1;
    }

    fread(data, 1, len, fp);
    data[len] = '\0';
    fclose(fp);

    cJSON *root = cJSON_Parse(data);
    free(data);

    if (!root) {
        fprintf(stderr, "JSON 解析失败\n");
        return -1;
    }

    cJSON *err_obj = cJSON_GetObjectItem(root, "512ErrorCode");
    if (!err_obj || !cJSON_IsObject(err_obj)) {
        fprintf(stderr, "找不到 '512ErrorCode' 字段\n");
        cJSON_Delete(root);
        return -1;
    }

    table->count = 0;
    cJSON *item = err_obj->child;
    while (item != NULL) {
        if (cJSON_IsString(item)) {
            int code = strtol(item->string, NULL, 16);
            if (table->count < MAX_ERROR_COUNT) {
                table->list[table->count].code = code;
                strncpy(table->list[table->count].desc, item->valuestring, MAX_DESC_LEN - 1);
                table->list[table->count].desc[MAX_DESC_LEN - 1] = '\0';
                table->count++;
            }
        }
        item = item->next;
    }

    cJSON_Delete(root);

    // 排序
    qsort(table->list, table->count, sizeof(ErrorEntry), compare_error_entry);

    for(int idx = 0; idx < table->count; ++idx)
    {
        printf("code[%d] code_desc[%d]:%s\n", table->list[idx].code, idx, table->list[idx].desc);
    }
    return 0;
}

// 二分查找错误描述
const char *get_error_description(const ErrorTable *table, int code) {
    int left = 0, right = table->count - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        printf("mid[%d] code[%d] code_desc = %s\n", mid, table->list[mid].code, table->list[mid].desc);
        if (table->list[mid].code == code) {
            return table->list[mid].desc;
        } else if (table->list[mid].code < code) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return "未知错误码";
}

int main() {
    ErrorTable table;

    if (load_error_codes("512ErrorCode.json", &table) != 0) {
        fprintf(stderr, "加载错误码配置失败\n");
        return 1;
    }

    // int test_codes[] = {0, 1, 2, 10, 99};
    // for (int i = 0; i < sizeof(test_codes)/sizeof(test_codes[0]); ++i) {
    //     printf("错误码 %d: %s\n", test_codes[i], get_error_description(&table, test_codes[i]));
    // }
    printf("0x17 desc:%s\n", get_error_description(&table, 23));
    printf("0xd4 desc:%s\n", get_error_description(&table, 212));
    printf("0x19 desc:%s\n", get_error_description(&table, 25));
    printf("0x00 desc:%s\n", get_error_description(&table, 0));
    printf("0xe1 desc:%s\n", get_error_description(&table, 225));

    return 0;
}

