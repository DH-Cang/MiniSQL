//2021/6/28 1:00
//数据存储方式如下
//叶子
//容量 数量 是否叶子 父亲节点
//数据  指针  指针 数据 .......  指针（指向右边）

//非叶子
//容量 数量 是否叶子 父亲节点       ，并非传统意义上的二叉树
//数据 指针 数据(后一个指针里面的最小数据) 指针 数据 .......  指针（指向右边的块，没有块值为0）

#include"IndexManager.h"
#include"RecordManager.h"
using namespace std;
float int_to_float(int a) {
	int b = a;
	float * ret = (float*)&b;
	return *ret;
}
int float_to_int(float a) {
	float b = a;
	int * ret = (int*)&b;
	return *ret;
}

void string_to_int(string test, int *a) {
	char * ss = (char *)a;
	int i;
	char tmp;
	int length = test.size();
	for (i = 0; i < 256; i++) {
		if (i < length) {
			tmp = test[i];
		}
		else {
			tmp = 0;
		}
		ss[i] = tmp;
	}
	a = (int*)a;
	// for (i = 0; i < 64; i++) {
	// 	cout << a[i] << " ";
	// }
	// cout << endl;
	//return a;
}
string int_to_string(int *a) {
	string output;
	output.resize(256);
	int i;
	char *ss;
	ss = (char*)a;
	for (i = 0; i < 256; i++) {
		if (ss[i] == 0) {
			break;
		}
		else {
			output[i] = ss[i];
		}
	}
	output.resize(i);
	a = (int *)a;
	// cout << output << endl;
	return output;
}
//all functions
//1 void Create_index(Index &index_info,Table &table, Buffer & buffer1)
/* */void Create_index_int(Index &index_info, Table &table, Buffer & buffer1);
/*	  */void Recreate_Int_Index(map<int, RecordAccess>& records, Index &index_info, Buffer &buffer1);
/*		  */void Store_Leaf_Node(Buffer &buffer1, vector <BlockID> &block_ptr2, int size_node, int capacity, map< int, RecordAccess >::iterator &records_iter);
/*		  */void Store_NonLeaf_Node(Buffer &buffer1, vector <BlockID> &block_ptr3, int size_node, int capacity, vector <BlockID>::iterator &block_iter);
/*		  */void Node_Sibling(Buffer & buffer1, vector <BlockID>& block_ptr2, int size);
/*-----------*/
/* */void Create_index_float(Index &index_info, Table &table, Buffer & buffer1);
/*	  */void Recreate_Float_Index(map<float, RecordAccess>& records, Index &index_info, Buffer &buffer1);
/*		  */void Store_Leaf_Node_F(Buffer &buffer1, vector <BlockID> &block_ptr2, int size_node, int capacity, map< float, RecordAccess >::iterator &records_iter);
/*		*///void Store_NonLeaf_Node(Buffer &buffer1,vector <Block*> &block_ptr3,int size_node,int capacity,vector <Block*>::iterator &block_iter);
/*		*///void Node_Sibling(vector <Block*>& block_ptr2,int size);
/*-----------*/
/* */void Create_index_string(Index &index_info, Table &table, Buffer & buffer1);
/*	  */void Recreate_String_Index(map<string, RecordAccess>& records, Index &index_info, Buffer &buffer1);
/*		  */void Store_Leaf_Node_STR(Buffer &buffer1, vector <BlockID> &block_ptr2, int size_node, int capacity, map< string, RecordAccess >::iterator &records_iter);
/*		  */void Store_NonLeaf_Node_STR(Buffer &buffer1, vector <BlockID> &block_ptr3, int size_node, int capacity, vector <BlockID>::iterator &block_iter);
/*		*///void Node_Sibling(vector <Block*>& block_ptr2,int size);
/*------------------------------------*/
//2 void Drop_index(Index &index_info, Buffer & buffer1);            
/*------------------------------------*/
//3 void index_Find(Index& index_info,Buffer & buffer1,Value & find_value);
/* */void index_Find_RecordAccess(Index& index_info, Buffer & buffer1, Value & find_value, vector<RecordAccess> &result3);
/*	  */void Find_index_int(Index& index_info, Buffer & buffer1, int find_value, vector<RecordAccess>& result, int type);
/*		  */void Iterate_Leaf(Index& index_info, Buffer & buffer1, int find_value, vector<RecordAccess>& result, int type);
/*		  */void Judge_type_access(Block * block, vector<RecordAccess>& result, int find_value, int type, Buffer & buffer1);
/*-----------*/
/* */void Find_index_float(Index& index_info, Buffer & buffer1, float find_value, vector<RecordAccess>& result, int type);
/*	  */void Find_index_float(Index& index_info, Buffer & buffer1, float find_value, vector<RecordAccess>& result, int type);
/*		  */void Iterate_Leaf_F(Index& index_info, Buffer & buffer1, float find_value, vector<RecordAccess>& result, int type);
/*		  */void Judge_type_access_F(Block * block, vector<RecordAccess>& result, float find_value, int type, Buffer & buffer1);
/*-----------*/
/* */void Find_index_string(Index& index_info, Buffer & buffer1, string find_value, vector<RecordAccess>& result, int type);
/*	  */void Find_index_string(Index& index_info, Buffer & buffer1, string find_value, vector<RecordAccess>& result, int type);
/*		  */void Judge_type_access_STR(Block * block, vector<RecordAccess>& result, string find_value, int type, Buffer & buffer1);
/*		  */void Iterate_Leaf_STR(Index& index_info, Buffer & buffer1, string find_value, vector<RecordAccess>& result, int type);
/*------------------------------------*/
//4 int index_Delete_record(Index& index_info,Table &table,Value& delete_value,Buffer & buffer1);
	//index_Find_RecordAccess(index_info,buffer1,delete_value,delete_record);
	//Drop_index(index_info,buffer1);
	//Create_index(index_info,table,buffer1);
/*------------------------------------*/
//5 void index_Insert_record(Index& index_info,Table &table,RecordAccess new_record,Buffer & buffer1);
/* */void Insert_int(Index& index_info, Table &table, RecordAccess new_record, Buffer & buffer1);
/*	*///Drop_index(index_info,buffer1);
/*	*///Create_index(index_info,table,buffer1);
/*	*///Store_Leaf_Node(buffer1,block_nouse,size+1,capacity,new_iter);
/*-----------*/
/* */void Insert_float(Index& index_info, Table &table, RecordAccess new_record, Buffer & buffer1);
/*-----------*/
/* */void Insert_string(Index& index_info, Table &table, RecordAccess new_record, Buffer & buffer1);


//还有两个分支没写
void Create_index(Index &index_info, Table &table, Buffer & buffer1) {
	int type = index_info.AttributeType;
	if (type == -1) Create_index_int(index_info, table, buffer1);
	else if (type == 0) Create_index_float(index_info, table, buffer1);
	else Create_index_string(index_info, table, buffer1);
}
void Create_index_string(Index &index_info, Table &table, Buffer & buffer1) {
	int record_offset = index_info.AttributeOffset;
	//获取数据
	RecordAccess place_record;//获取位置
	place_record.bid = table.BlockNumber;
	BlockID block_id = place_record.bid;
	Block * record_block = buffer1.Retrieve(block_id);
	BlockHead block_head;//获取头部
	block_head.Read((*record_block));
	place_record.offset = block_head.RecordPtr;
	Record each_record = place_record.Read(buffer1);

	map< string, RecordAccess > records;
	//int each_int_attribute[64];
	//int i;
	string each_string_attribute;
	int number = 0;

	while (1) {
		while (1) {
			if (place_record.offset == NULL_offset) break;
			each_string_attribute = each_record.GetString(record_offset);//获取记录
			records.insert(pair<string, RecordAccess>(each_string_attribute, place_record));
			//下一条数据
			place_record.offset = each_record.GetNext();

			each_record = place_record.Read(buffer1);
		}
		block_id = block_head.NextBlock;//更新block号：int
		if (block_id == NULL_Block) break;
		record_block = buffer1.Retrieve(block_id);//获取新的block的内容：Block*
		block_head.Read((*record_block));//获取新的记录头：BlockHead
		//RecordAccess的更新
		place_record.bid = block_id;
		place_record.offset = block_head.RecordPtr;
		//对现在的数据的更新
		each_record = place_record.Read(buffer1);
	}
	//通过map存储记录信息
	//操作： 通过record的首记录来不断插入
	//操作： 统计记录数量
	//---------------------done----------------------------
	//建立map的迭代器
	Recreate_String_Index(records, index_info, buffer1);
}
void Recreate_String_Index(map<string, RecordAccess>& records, Index &index_info, Buffer &buffer1) {
	//叶子 
	int size_leaf = 256 + 4 + 4;
	int capacity_leaf = (BLOCK_SIZE - 4 - 16) / size_leaf;//254
			/*int*/ /*RecordAccess*/
	//非叶子
	int size_nonleaf = 256 + 4;
	int capacity_nonleaf = (BLOCK_SIZE - 4 - 16) / size_nonleaf;//509
			/*block_id*/ /*int*/
	//头部要存储容量(4)、存储数量(4)、是否是叶子（4）、父亲节点块号（4）
	//计算： 计算root的capacity
	int number = records.size();
	//leaf
	int expected_leaf = 2.0 / 3.0 * capacity_leaf;//按照2/3进行存储
	int number_leaf = number / (expected_leaf);
	map< string, RecordAccess >::iterator records_iter = records.begin();
	//不断地存储
	int iter_number;
	vector <BlockID> block_ptr2;
	int leaf_number = 0;
	int height = 0;
	int size = sizeof(int);
	while (leaf_number < number_leaf) {
		Store_Leaf_Node_STR(buffer1, block_ptr2, expected_leaf, capacity_leaf, records_iter);
		//finished storage of this node
		//遍历写入，并记住位置，下次继续写入
		leaf_number++;
	}
	//最后一个节点大小不足
	int left_number = number - leaf_number * expected_leaf;
	if (left_number > 0)Store_Leaf_Node_STR(buffer1, block_ptr2, left_number, capacity_leaf, records_iter);
	if (block_ptr2.size() == 1) {
		index_info.BlockNumber = block_ptr2[0];
		Node_Sibling(buffer1, block_ptr2, 264);
		return;
	}
	else if (block_ptr2.size() < 1) {
		//first
		Store_Leaf_Node_STR(buffer1, block_ptr2, left_number, capacity_leaf, records_iter);
		index_info.BlockNumber = block_ptr2[0];
		Node_Sibling(buffer1, block_ptr2, 264);
		return;
	}
	Node_Sibling(buffer1, block_ptr2, 264);

	//之后存上一层，跟这层差不多只不过要存儿子父亲节点块号

//nonleaf
	number = block_ptr2.size();
	vector <BlockID> block_ptr3;//father node
	int expected_nonleaf = 2.0 / 3.0 * capacity_nonleaf;
	int number_node = number / (expected_nonleaf);
	int node_number = 0;
	vector <BlockID>::iterator block_iter = block_ptr2.begin();
	while (1) {
		while (node_number < number_node) {
			Store_NonLeaf_Node_STR(buffer1, block_ptr3, expected_nonleaf, capacity_nonleaf, block_iter);
			node_number++;
		}
		left_number = number - expected_nonleaf * number_node;
		if(left_number > 0)Store_NonLeaf_Node_STR(buffer1, block_ptr3, left_number, capacity_nonleaf, block_iter);
		if (block_ptr3.size() == 1) {
			index_info.BlockNumber = block_ptr3[0];
			Node_Sibling(buffer1, block_ptr3, 260);
			return;
		}
		else if (block_ptr3.size() < 1) {
			//error
		}
		Node_Sibling(buffer1, block_ptr3, 260);

		number = block_ptr3.size();
		number_node = number / (expected_nonleaf);
		node_number = 0;
		block_ptr3.swap(block_ptr2);    //block_ptr3交换到block_ptr2
		vector <BlockID>().swap(block_ptr3);//清空block_ptr3
		block_iter = block_ptr2.begin();
	}
}

void Store_NonLeaf_Node_STR(Buffer &buffer1, vector <BlockID> &block_ptr3,
	int size_node, int capacity, vector <BlockID>::iterator &block_iter) {
	int iter_number = 0;
	Block* newnode = buffer1.GetEmptyBlock();
	block_ptr3.push_back(newnode->GetID());//存储这个节点的指针
	(*newnode).WriteInt(0 * 4, capacity);
	(*newnode).WriteInt(1 * 4, size_node);
	(*newnode).WriteInt(2 * 4, 0);
	(*newnode).WriteInt(3 * 4, 0);//父节点
	int size = 260;
	Block * tmp1; Block *tmp2;
	int i = 0;
	while (iter_number < size_node) {
		tmp1 = buffer1.Retrieve(*block_iter);
		//
		if ((tmp1)->ReadInt(4) != 0) {
			for (i = 0; i < 64; i++) {
				(*newnode).WriteInt((iter_number)*size + 4 * i + 16, (tmp1)->ReadInt(16 + 4 * i));
			}
			//
			(*newnode).WriteInt((iter_number)*size + 256 + 16, (tmp1)->GetID());
			(tmp1)->WriteInt(3 * 4, (*newnode).GetID());
			tmp1->DirtyBit = 1;
			newnode = buffer1.Retrieve(newnode->GetID());
		}
		newnode = buffer1.Retrieve(newnode->GetID());
		block_iter++;
		iter_number++;
	}
}
//
void Store_Leaf_Node_STR(Buffer &buffer1, vector <BlockID> &block_ptr2,
	int size_node, int capacity, map< string, RecordAccess >::iterator &records_iter) {
	int iter_number = 0;
	Block* newleaf = buffer1.GetEmptyBlock();
	block_ptr2.push_back(newleaf->GetID());//存储这个节点的指针
	(*newleaf).WriteInt(0 * 4, capacity);
	(*newleaf).WriteInt(1 * 4, size_node);

	(*newleaf).WriteInt(2 * 4, 1);
	(*newleaf).WriteInt(3 * 4, 0);//父节点
	int size = 264;
	int write[64];
	int i = 0;
	//循环写入
	while (iter_number < size_node) {
		//
		string_to_int(records_iter->first, write);
		for (i = 0; i < 64; i++) {
			(*newleaf).WriteInt((iter_number)*size + 4 * i + 16, write[i]);
		}
		//
		(*newleaf).WriteInt((iter_number)*size + 256 + 16, records_iter->second.bid);
		(*newleaf).WriteInt((iter_number)*size + 260 + 16, records_iter->second.offset);
		newleaf = buffer1.Retrieve(newleaf->GetID());
		records_iter++;
		iter_number++;
	}
}
//
void Create_index_int(Index &index_info, Table &table, Buffer & buffer1) {

	int record_offset = index_info.AttributeOffset;
	//获取数据
	RecordAccess place_record;//获取位置
	place_record.bid = table.BlockNumber;
	BlockID block_id = place_record.bid;
	Block * record_block = buffer1.Retrieve(block_id);
	BlockHead block_head;//获取头部
	block_head.Read((*record_block));
	place_record.offset = block_head.RecordPtr;
	Record each_record = place_record.Read(buffer1);

	map< int, RecordAccess > records;
	int each_int_attribute;
	int number = 0;

	while (1) {
		while (1) {
			if (place_record.offset == NULL_offset) break;
			each_int_attribute = each_record.GetInt(record_offset);//获取记录
			records.insert(pair<int, RecordAccess>(each_int_attribute, place_record));
			//下一条数据
			place_record.offset = each_record.GetNext();

			each_record = place_record.Read(buffer1);
		}
		block_id = block_head.NextBlock;//更新block号：int
		if (block_id == NULL_Block) break;
		record_block = buffer1.Retrieve(block_id);//获取新的block的内容：Block*
		block_head.Read((*record_block));//获取新的记录头：BlockHead
		//RecordAccess的更新
		place_record.bid = block_id;
		place_record.offset = block_head.RecordPtr;
		//对现在的数据的更新
		each_record = place_record.Read(buffer1);
	}
	//通过map存储记录信息
	//操作： 通过record的首记录来不断插入
	//操作： 统计记录数量
	//---------------------done----------------------------
	//建立map的迭代器
	Recreate_Int_Index(records, index_info, buffer1);
}
//int的操作
void Create_index_float(Index &index_info, Table &table, Buffer & buffer1) {
	int record_offset = index_info.AttributeOffset;
	//获取数据
	RecordAccess place_record;//获取位置
	place_record.bid = table.BlockNumber;
	BlockID block_id = place_record.bid;
	Block * record_block = buffer1.Retrieve(block_id);
	BlockHead block_head;//获取头部
	block_head.Read((*record_block));
	place_record.offset = block_head.RecordPtr;
	Record each_record = place_record.Read(buffer1);

	map< float, RecordAccess > records;
	int each_int_attribute;
	float each_float_attribute;//改动
	int number = 0;

	while (1) {
		while (1) {
			if (place_record.offset == NULL_offset) break;
			each_int_attribute = each_record.GetInt(record_offset);//获取记录
			//改动
			each_float_attribute = int_to_float(each_int_attribute);
			records.insert(pair<float, RecordAccess>(each_float_attribute, place_record));
			//下一条数据
			place_record.offset = each_record.GetNext();

			each_record = place_record.Read(buffer1);
		}
		block_id = block_head.NextBlock;//更新block号：int
		if (block_id == NULL_Block) break;
		record_block = buffer1.Retrieve(block_id);//获取新的block的内容：Block*
		block_head.Read((*record_block));//获取新的记录头：BlockHead
		//RecordAccess的更新
		place_record.bid = block_id;
		place_record.offset = block_head.RecordPtr;
		//对现在的数据的更新
		each_record = place_record.Read(buffer1);
	}
	Recreate_Float_Index(records, index_info, buffer1);//改动
}
void Recreate_Float_Index(map<float, RecordAccess>& records, Index &index_info, Buffer &buffer1) {
	//叶子 
	int size_leaf = 4 + 4 + 4;
	int capacity_leaf = (BLOCK_SIZE - 4 - 16) / size_leaf;//254
			/*int*/ /*RecordAccess*/
	//非叶子
	int size_nonleaf = 4 + 4;
	int capacity_nonleaf = (BLOCK_SIZE - 4 - 16) / size_nonleaf;//509
			/*block_id*/ /*int*/
	//头部要存储容量(4)、存储数量(4)、是否是叶子（4）、父亲节点块号（4）
	//计算： 计算root的capacity
	int number = records.size();
	//leaf
	int expected_leaf = 2.0 / 3.0 * capacity_leaf;//按照2/3进行存储
	int number_leaf = number / (expected_leaf);
	map< float, RecordAccess >::iterator records_iter = records.begin();
	//不断地存储
	int iter_number;
	vector <BlockID> block_ptr2;
	int leaf_number = 0;
	int height = 0;
	int size = sizeof(float);
	while (leaf_number < number_leaf) {
		Store_Leaf_Node_F(buffer1, block_ptr2, expected_leaf, capacity_leaf, records_iter);
		//finished storage of this node
		//遍历写入，并记住位置，下次继续写入
		leaf_number++;
	}
	//最后一个节点大小不足
	int left_number = number - leaf_number * expected_leaf;
	if(left_number !=0 )	Store_Leaf_Node_F(buffer1, block_ptr2, left_number, capacity_leaf, records_iter);
	if (block_ptr2.size() == 1) {
		index_info.BlockNumber = block_ptr2[0];
		Node_Sibling(buffer1, block_ptr2, 12);
		return;
	}
	if (block_ptr2.size() == 0) {
		Store_Leaf_Node_F(buffer1, block_ptr2, left_number, capacity_leaf, records_iter);
		index_info.BlockNumber = block_ptr2[0];
		Node_Sibling(buffer1, block_ptr2, 12);
		return;
	}
	Node_Sibling(buffer1, block_ptr2, 12);

	//之后存上一层，跟这层差不多只不过要存儿子父亲节点块号

//nonleaf
	number = block_ptr2.size();
	vector <BlockID> block_ptr3;//father node
	int expected_nonleaf = 2.0 / 3.0 * capacity_nonleaf;
	int number_node = number / (expected_nonleaf);
	int node_number = 0;
	vector <BlockID>::iterator block_iter = block_ptr2.begin();
	while (1) {
		while (node_number < number_node) {
			Store_NonLeaf_Node(buffer1, block_ptr3, expected_nonleaf, capacity_nonleaf, block_iter);
			//这里这条函数好像可以一样
			node_number++;
		}
		if (left_number != 0) left_number = number - expected_nonleaf * number_node;
		Store_NonLeaf_Node(buffer1, block_ptr3, left_number, capacity_nonleaf, block_iter);
		if (block_ptr3.size() == 1) {
			index_info.BlockNumber = block_ptr3[0];
			Node_Sibling(buffer1, block_ptr3, 8);
			return;
		}
		Node_Sibling(buffer1, block_ptr3, 8);

		number = block_ptr3.size();
		number_node = number / (expected_nonleaf);
		node_number = 0;
		block_ptr3.swap(block_ptr2);    //block_ptr3交换到block_ptr2
		vector <BlockID>().swap(block_ptr3);//清空block_ptr3
		block_iter = block_ptr2.begin();
	}

	//操作： 逐层向上存index的node并且存入数据直到根节点
}
void Store_Leaf_Node_F(Buffer &buffer1, vector <BlockID> &block_ptr2,
	int size_node, int capacity, map<float, RecordAccess >::iterator &records_iter) {
	int iter_number = 0;
	Block* newleaf = buffer1.GetEmptyBlock();
	block_ptr2.push_back(newleaf->GetID());//存储这个节点的指针
	(*newleaf).WriteInt(0 * 4, capacity);
	(*newleaf).WriteInt(1 * 4, size_node);

	(*newleaf).WriteInt(2 * 4, 1);
	(*newleaf).WriteInt(3 * 4, 0);//父节点
	int size = 12;
	//循环写入
	int int_value;
	float float_value;
	while (iter_number < size_node) {
		float_value = records_iter->first;
		int_value = float_to_int(float_value);
		(*newleaf).WriteInt((iter_number)*size + 16, int_value);
		(*newleaf).WriteInt((iter_number)*size + 4 + 16, records_iter->second.bid);
		(*newleaf).WriteInt((iter_number)*size + 8 + 16, records_iter->second.offset);
		newleaf = buffer1.Retrieve(newleaf->GetID());
		records_iter++;
		iter_number++;
	}
}

//int 类型


//int 类型
void Recreate_Int_Index(map<int, RecordAccess>& records, Index &index_info, Buffer &buffer1) {
	//叶子 
	int size_leaf = 4 + 4 + 4;
	int capacity_leaf = (BLOCK_SIZE - 4 - 16) / size_leaf;//254
			/*int*/ /*RecordAccess*/
	//非叶子
	int size_nonleaf = 4 + 4;
	int capacity_nonleaf = (BLOCK_SIZE - 4 - 16) / size_nonleaf;//509
			/*block_id*/ /*int*/
	//头部要存储容量(4)、存储数量(4)、是否是叶子（4）、父亲节点块号（4）
	//计算： 计算root的capacity
	int number = records.size();
	//leaf
	int expected_leaf = 2.0 / 3.0 * capacity_leaf;//按照2/3进行存储
	int number_leaf = number / (expected_leaf);
	map< int, RecordAccess >::iterator records_iter = records.begin();
	//不断地存储
	int iter_number;
	vector <BlockID> block_ptr2;
	int leaf_number = 0;
	int height = 0;
	int size = sizeof(int);
	while (leaf_number < number_leaf) {
		Store_Leaf_Node(buffer1, block_ptr2, expected_leaf, capacity_leaf, records_iter);
		//finished storage of this node
		//遍历写入，并记住位置，下次继续写入
		leaf_number++;
	}
	//最后一个节点大小不足
	int left_number = number - leaf_number * expected_leaf;
	if (left_number != 0) Store_Leaf_Node(buffer1, block_ptr2, left_number, capacity_leaf, records_iter);
	if (block_ptr2.size() == 1) {
		index_info.BlockNumber = block_ptr2[0];
		Node_Sibling(buffer1, block_ptr2, 12);
		return;
	}
	if (block_ptr2.size() == 0) {
		Store_Leaf_Node(buffer1, block_ptr2, left_number, capacity_leaf, records_iter);
		index_info.BlockNumber = block_ptr2[0];
		Node_Sibling(buffer1, block_ptr2, 12);
		return;
	}
	Node_Sibling(buffer1, block_ptr2, 12);

	//之后存上一层，跟这层差不多只不过要存儿子父亲节点块号

//nonleaf
	number = block_ptr2.size();
	vector <BlockID> block_ptr3;//father node
	int expected_nonleaf = 2.0 / 3.0 * capacity_nonleaf;
	int number_node = number / (expected_nonleaf);
	int node_number = 0;
	vector <BlockID>::iterator block_iter = block_ptr2.begin();
	while (1) {
		while (node_number < number_node) {
			Store_NonLeaf_Node(buffer1, block_ptr3, expected_nonleaf, capacity_nonleaf, block_iter);
			node_number++;
		}
		if (left_number != 0) left_number = number - expected_nonleaf * number_node;
		Store_NonLeaf_Node(buffer1, block_ptr3, left_number, capacity_nonleaf, block_iter);
		if (block_ptr3.size() == 1) {
			index_info.BlockNumber = block_ptr3[0];
			Node_Sibling(buffer1, block_ptr3, 8);
			return;
		}
		Node_Sibling(buffer1, block_ptr3, 8);

		number = block_ptr3.size();
		number_node = number / (expected_nonleaf);
		node_number = 0;
		block_ptr3.swap(block_ptr2);    //block_ptr3交换到block_ptr2
		vector <BlockID>().swap(block_ptr3);//清空block_ptr3
		block_iter = block_ptr2.begin();
	}

	//操作： 逐层向上存index的node并且存入数据直到根节点
}

//int 类型
void Store_NonLeaf_Node(Buffer &buffer1, vector <BlockID> &block_ptr3,
	int size_node, int capacity, vector <BlockID>::iterator &block_iter) {
	int iter_number = 0;
	Block* newnode = buffer1.GetEmptyBlock();
	block_ptr3.push_back(newnode->GetID());//存储这个节点的指针
	(*newnode).WriteInt(0 * 4, capacity);
	(*newnode).WriteInt(1 * 4, size_node);
	(*newnode).WriteInt(2 * 4, 0);
	(*newnode).WriteInt(3 * 4, 0);//父节点
	int size = 8;
	Block * tmp1; Block *tmp2;
	while (iter_number < size_node) {
		tmp1 = buffer1.Retrieve(*block_iter);
		(*newnode).WriteInt((iter_number)*size + 16, (tmp1)->ReadInt(16));
		(*newnode).WriteInt((iter_number)*size + 4 + 16, (tmp1)->GetID());
		(tmp1)->WriteInt(3 * 4, (*newnode).GetID());
		tmp1->DirtyBit = 1;
		newnode = buffer1.Retrieve(newnode->GetID());
		block_iter++;
		iter_number++;
	}
	//end
}

//通用，需要参数size：12/8 12/8 264/260
void Node_Sibling(Buffer & buffer1, vector <BlockID>& block_ptr2, int size) {//size is the bytes each record occupy
	vector <BlockID>::iterator iter_front = block_ptr2.begin();
	vector <BlockID>::iterator iter_back = block_ptr2.begin();
	iter_back++;
	int position;
	Block * tmp1; Block *tmp2;
	while (iter_back != block_ptr2.end()) {//To store the blockid of siblings

		tmp1 = buffer1.Retrieve(*iter_front);
		tmp1->Lock = true;
		tmp1->DirtyBit = 1;
		tmp2 = buffer1.Retrieve(*iter_back);
		tmp2->Lock = true;
		position = (*tmp1).ReadInt(4);
		position = position * size + 16;
		(tmp1)->WriteInt(position, (*iter_back));
		iter_back++;
		iter_front++;
		tmp1->Lock = false;
		tmp2->Lock = false;
	}
	tmp1 = buffer1.Retrieve(*iter_front);
	tmp1->DirtyBit = 1;
	position = (tmp1)->ReadInt(4);
	position = position * size + 16;
	(tmp1)->WriteInt(position, 0);

	//end
}

//int 类型
void Store_Leaf_Node(Buffer &buffer1, vector <BlockID> &block_ptr2,
	int size_node, int capacity, map< int, RecordAccess >::iterator &records_iter) {
	int iter_number = 0;
	Block* newleaf = buffer1.GetEmptyBlock();
	block_ptr2.push_back(newleaf->GetID());//存储这个节点的指针
	(*newleaf).WriteInt(0 * 4, capacity);
	(*newleaf).WriteInt(1 * 4, size_node);

	(*newleaf).WriteInt(2 * 4, 1);
	(*newleaf).WriteInt(3 * 4, 0);//父节点
	int size = 12;
	//循环写入
	while (iter_number < size_node) {
		(*newleaf).WriteInt((iter_number)*size + 16, records_iter->first);
		(*newleaf).WriteInt((iter_number)*size + 4 + 16, records_iter->second.bid);
		(*newleaf).WriteInt((iter_number)*size + 8 + 16, records_iter->second.offset);
		newleaf = buffer1.Retrieve(newleaf->GetID());
		records_iter++;
		iter_number++;
	}
}

//将index的block全部扔掉，无类型drop表
void Drop_index(Index &index_info, Buffer & buffer1) {
	BlockID root = index_info.BlockNumber;
	vector <BlockID> left_blocks;
	left_blocks.push_back(root);
	BlockID father = root;
	int isleaf;
	int size;
	if (index_info.AttributeType <= 0) size = 4;
	else size = 256 ;
	Block * left_block;
	while (1) {
		left_block = buffer1.Retrieve(father);
		isleaf = (*left_block).ReadInt(8);
		if (isleaf) {
			break;
		}
		father = (*left_block).ReadInt(16+size);
		left_blocks.push_back(father);
	}//读取了所有最左边的节点
	//逐层删除
	vector <BlockID>::iterator drop_iter = left_blocks.end();
	drop_iter--;
	int position;
	Block *block;

	BlockID next;
	if (index_info.AttributeType <= 0) size = 12;
	else size = 256 + 8;
	while (drop_iter >= left_blocks.begin()) {
		block = buffer1.Retrieve(*drop_iter);
		position = (*block).ReadInt(4);
		position = position * size + 16;
		next = (*block).ReadInt(position);
		block->DirtyBit = 1;
		//clear the block
		buffer1.SetEmpty(block->GetID());

		while (next != 0) {
			block = buffer1.Retrieve(next);
			position = (*block).ReadInt(4);
			position = position * size + 16;
			next = (*block).ReadInt(position);
			block->DirtyBit = 1;
			//clear the block
			buffer1.SetEmpty(block->GetID());

		}
		if (index_info.AttributeType <= 0) size = 8;
		else size = 256 + 4;
		if (drop_iter == left_blocks.begin()) break;
		drop_iter--;
	}

	index_info.BlockNumber = 0;

}

//找到某个信息的叶子节点

//无类型调用接口
void index_Find(Index& index_info, Value& find_value, Buffer& buffer1) {
	//get the value to recordaccess
	vector<RecordAccess> result_RecordAccess;
	index_Find_RecordAccess(index_info, buffer1, find_value, result_RecordAccess);
	//get the value to record
	vector<RecordAccess> ::iterator result_RecordAccess_iter;
	for (result_RecordAccess_iter = result_RecordAccess.begin(); result_RecordAccess_iter != result_RecordAccess.end(); result_RecordAccess_iter++) {
		find_value.search_result.push_back((*result_RecordAccess_iter).Read(buffer1));
	}
	//end
}

//还有两个分支没写
void index_Find_RecordAccess(Index& index_info, Buffer & buffer1, Value & find_value, vector<RecordAccess> &result3) {
	vector<RecordAccess> result1;
	vector<RecordAccess> result2;
	if (find_value.number == 1) {
		if (index_info.AttributeType < 0)
			Find_index_int(index_info, buffer1, find_value.int_value[0], result3, find_value.type[0]);
		else if (index_info.AttributeType == 0) {
			Find_index_float(index_info, buffer1, find_value.float_value[0], result3, find_value.type[0]);
		}
		else {
			Find_index_string(index_info, buffer1, find_value.string_value[0], result3, find_value.type[0]);
			//cout << find_value.search_result.size();
			//cout << find_value.search_result[0].GetString(index_info.AttributeOffset) << endl;;
		}
			
	}
	else if (find_value.number == 2) {
		if (index_info.AttributeType < 0) {
			Find_index_int(index_info, buffer1, find_value.int_value[0], result1, find_value.type[0]);
			Find_index_int(index_info, buffer1, find_value.int_value[1], result2, find_value.type[1]);
			sort(result1.begin(), result1.end());
			sort(result2.begin(), result2.end());
			set_intersection(result1.begin(), result1.end(), result2.begin(), result2.end(), inserter(result3, result3.begin()));
		}
		else if (index_info.AttributeType == 0) {
			Find_index_float(index_info, buffer1, find_value.float_value[0], result1, find_value.type[0]);
			Find_index_float(index_info, buffer1, find_value.float_value[1], result2, find_value.type[1]);
			sort(result1.begin(), result1.end());
			sort(result2.begin(), result2.end());
			set_intersection(result1.begin(), result1.end(), result2.begin(), result2.end(), inserter(result3, result3.begin()));
		}
		else {
			Find_index_string(index_info, buffer1, find_value.string_value[0], result1, find_value.type[0]);
			Find_index_string(index_info, buffer1, find_value.string_value[1], result2, find_value.type[1]);
			sort(result1.begin(), result1.end());
			sort(result2.begin(), result2.end());
			set_intersection(result1.begin(), result1.end(), result2.begin(), result2.end(), inserter(result3, result3.begin()));
		}
	}
}
void Find_index_string(Index& index_info, Buffer & buffer1, string find_value,
	vector<RecordAccess>& result, int type) {
	if (type > 0) {
		Iterate_Leaf_STR(index_info, buffer1, find_value, result, type);
		return;
	}

	BlockID root = index_info.BlockNumber;
	vector <int> index_value;
	BlockID father = root;
	int isleaf;
	int children_number;
	Block * block;
	string value_str;
	int value[64]; int i = 0;
	int number = 0;
	while (1) {
		block = buffer1.Retrieve(father);
		isleaf = (*block).ReadInt(8);
		if (isleaf) {
			break;
		}
		children_number = (*block).ReadInt(4);
		for (number = 0; number < children_number; number++) {
			//
			for (i = 0; i < 64; i++) {
				value[i] = (*block).ReadInt(16 + number * 260 + 4 * i);
			}
			value_str = int_to_string(value);
			//
			if (value_str > find_value) {
				break;
			}
		}
		//judge
		if (number == 0) {
			father = (*block).ReadInt(16 + 256);
		}
		else {
			father = (*block).ReadInt(16 + number * 260 - 4);
		}

	}//读取了所有非叶子节点
	//叶子
	Judge_type_access_STR(block, result, find_value, type, buffer1);
}
void Iterate_Leaf_STR(Index& index_info, Buffer & buffer1, string find_value,
	vector<RecordAccess>& result, int type) {
	//找到最左边叶子
	BlockID root = index_info.BlockNumber;
	BlockID father = root;
	int isleaf;
	Block * block;
	while (1) {
		block = buffer1.Retrieve(father);//最后这个就是最左边叶节点
		isleaf = (*block).ReadInt(8);
		if (isleaf) {
			break;
		}
		father = (*block).ReadInt(16+256);
	}
	//遍历
	int number;
	RecordAccess tmp;
	string value_str;
	int value[64]; int i = 0;
	int block2 = (*block).GetID();
	if (type == 1) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				//
				for (i = 0; i < 64; i++) {
					value[i] = (*block).ReadInt(16 + number * 264 + 4 * i);
				}
				value_str = int_to_string(value);
				//
				if (value_str > find_value) {
					return;
				}
				tmp.bid = (*block).ReadInt(16 + number * 264 + 256);
				tmp.offset = (*block).ReadInt(16 + number * 264 + 260);
				result.push_back(tmp);
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 264);

		}
	}
	if (type == 2) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				//
				for (i = 0; i < 64; i++) {
					value[i] = (*block).ReadInt(16 + number * 264 + 4 * i);
				}
				value_str = int_to_string(value);
				//
				if (value_str >= find_value) {
					return;
				}
				tmp.bid = (*block).ReadInt(16 + number * 264 + 256);
				tmp.offset = (*block).ReadInt(16 + number * 264 + 260);
				result.push_back(tmp);
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 264);

		}
	}

}
//
void Judge_type_access_STR(Block * block, vector<RecordAccess>& result, string find_value,
	int type, Buffer & buffer1) {
	int number = 0;
	string value_str;
	int value[64]; int i = 0;
	RecordAccess tmp;
	if (type == 0) {
		while (number < (*block).ReadInt(4)) {
			//
			for (i = 0; i < 64; i++) {
				value[i] = (*block).ReadInt(16 + number * 264 + 4 * i);
			}
			value_str = int_to_string(value);
			//
			if (value_str == find_value) {
				
				tmp.bid = (*block).ReadInt(16 + number * 264 + 256);
				tmp.offset = (*block).ReadInt(16 + number * 264 + 260);
				result.push_back(tmp);
				return;
			}
			number++;
		}
	}
	BlockID block2 = (*block).GetID();
	if (type == -1) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				//
				for (i = 0; i < 64; i++) {
					value[i] = (*block).ReadInt(16 + number * 264 + 4 * i);
				}
				value_str = int_to_string(value);
				//
				if (value_str >= find_value) {
					tmp.bid = (*block).ReadInt(16 + number * 264 + 256);
					tmp.offset = (*block).ReadInt(16 + number * 264 + 260);
					result.push_back(tmp);
				}
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 264);

		}
	}
	if (type == -2) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				//
				for (i = 0; i < 64; i++) {
					value[i] = (*block).ReadInt(16 + number * 264 + 4 * i);
				}
				value_str = int_to_string(value);
				//
				if (value_str > find_value) {
					tmp.bid = (*block).ReadInt(16 + number * 264 + 256);
					tmp.offset = (*block).ReadInt(16 + number * 264 + 260);
					result.push_back(tmp);

				}
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 264);

		}
	}
}
//
void Find_index_float(Index& index_info, Buffer & buffer1, float find_value,
	vector<RecordAccess>& result, int type) {
	if (type > 0) {
		Iterate_Leaf_F(index_info, buffer1, find_value, result, type);
		return;
	}

	BlockID root = index_info.BlockNumber;
	vector <int> index_value;
	BlockID father = root;
	int isleaf;
	int children_number;
	//
	int int_v;
	float float_v;
	//
	Block * block;
	int number = 0;
	while (1) {
		block = buffer1.Retrieve(father);
		isleaf = (*block).ReadInt(8);
		if (isleaf) {
			break;
		}
		children_number = (*block).ReadInt(4);
		for (number = 0; number < children_number; number++) {
			int_v = (*block).ReadInt(16 + number * 8);
			float_v = int_to_float(int_v);
			if (float_v > find_value) {
				break;
			}
		}
		//judge
		if (number == 0) {
			father = (*block).ReadInt(20);
		}
		else {
			father = (*block).ReadInt(16 + number * 8 - 4);
		}

	}//读取了所有非叶子节点
	//叶子
	Judge_type_access_F(block, result, find_value, type, buffer1);
}
void Judge_type_access_F(Block * block, vector<RecordAccess>& result, float find_value,
	int type, Buffer & buffer1) {
	int number = 0;
	RecordAccess tmp;
	int int_v;
	float float_v;
	if (type == 0) {
		while (number < (*block).ReadInt(4)) {
			int_v = (*block).ReadInt(16 + number * 12);
			float_v = int_to_float(int_v);
			if (float_v == find_value) {
				tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
				tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
				result.push_back(tmp);
				return;
			}
			number++;
		}
	}
	BlockID block2 = (*block).GetID();
	if (type == -1) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				int_v = (*block).ReadInt(16 + number * 12);
				float_v = int_to_float(int_v);
				if (float_v >= find_value) {
					tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
					tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
					result.push_back(tmp);
				}
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 12);

		}
	}
	if (type == -2) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				int_v = (*block).ReadInt(16 + number * 12);
				float_v = int_to_float(int_v);
				if (float_v > find_value) {
					tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
					tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
					result.push_back(tmp);

				}
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 12);

		}
	}
}
void Find_index_int(Index& index_info, Buffer & buffer1, int find_value,
	vector<RecordAccess>& result, int type) {
	if (type > 0) {
		Iterate_Leaf(index_info, buffer1, find_value, result, type);
		return;
	}

	BlockID root = index_info.BlockNumber;
	vector <int> index_value;
	BlockID father = root;
	int isleaf;
	int children_number;
	Block * block;
	int number = 0;
	while (1) {
		block = buffer1.Retrieve(father);
		isleaf = (*block).ReadInt(8);
		if (isleaf) {
			break;
		}
		children_number = (*block).ReadInt(4);
		for (number = 0; number < children_number; number++) {
			if ((*block).ReadInt(16 + number * 8) > find_value) {
				break;
			}
		}
		//judge
		if (number == 0) {
			father = (*block).ReadInt(16 + 4);
		}
		else {
			father = (*block).ReadInt(16 + number * 8 - 4);
		}

	}//读取了所有非叶子节点
	//叶子
	Judge_type_access(block, result, find_value, type, buffer1);
}
void Iterate_Leaf_F(Index& index_info, Buffer & buffer1, float find_value,
	vector<RecordAccess>& result, int type) {
	//找到最左边叶子
	BlockID root = index_info.BlockNumber;
	BlockID father = root;
	int isleaf;
	Block * block;
	int int_v;
	float float_v;
	while (1) {
		block = buffer1.Retrieve(father);//最后这个就是最左边叶节点
		isleaf = (*block).ReadInt(8);
		if (isleaf) {
			break;
		}
		father = (*block).ReadInt(20);
	}
	//遍历
	int number;

	RecordAccess tmp;
	int block2 = (*block).GetID();
	if (type == 1) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				int_v = (*block).ReadInt(16 + number * 12);
				float_v = int_to_float(int_v);
				if (float_v > find_value) {
					return;
				}
				tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
				tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
				result.push_back(tmp);
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 12);

		}
	}
	if (type == 2) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				int_v = (*block).ReadInt(16 + number * 12);
				float_v = int_to_float(int_v);
				if (float_v >= find_value) {
					return;
				}
				tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
				tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
				result.push_back(tmp);
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 12);

		}
	}

}
//int型 index_value <= < int
void Iterate_Leaf(Index& index_info, Buffer & buffer1, int find_value,
	vector<RecordAccess>& result, int type) {
	//找到最左边叶子
	BlockID root = index_info.BlockNumber;
	BlockID father = root;
	int isleaf;
	Block * block;
	while (1) {
		block = buffer1.Retrieve(father);//最后这个就是最左边叶节点
		isleaf = (*block).ReadInt(8);
		if (isleaf) {
			break;
		}
		father = (*block).ReadInt(20);
	}
	//遍历
	int number;
	RecordAccess tmp;
	int block2 = (*block).GetID();
	if (type == 1) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				if ((*block).ReadInt(16 + number * 12) > find_value) {
					return;
				}
				tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
				tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
				result.push_back(tmp);
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 12);

		}
	}
	if (type == 2) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				if ((*block).ReadInt(16 + number * 12) >= find_value) {
					return;
				}
				tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
				tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
				result.push_back(tmp);
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 12);

		}
	}

}
//int型 index_value > >= ==
void Judge_type_access(Block * block, vector<RecordAccess>& result, int find_value,
	int type, Buffer & buffer1) {
	int number = 0;
	RecordAccess tmp;
	if (type == 0) {
		while (number < (*block).ReadInt(4)) {
			if ((*block).ReadInt(16 + number * 12) == find_value) {
				tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
				tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
				result.push_back(tmp);
				return;
			}
			number++;
		}
	}
	BlockID block2 = (*block).GetID();
	if (type == -1) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				if ((*block).ReadInt(16 + number * 12) >= find_value) {
					tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
					tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
					result.push_back(tmp);
				}
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 12);

		}
	}
	if (type == -2) {
		while (block2 != 0) {
			block = buffer1.Retrieve(block2);
			number = 0;
			while (number < (*block).ReadInt(4)) {
				if ((*block).ReadInt(16 + number * 12) > find_value) {
					tmp.bid = (*block).ReadInt(16 + number * 12 + 4);
					tmp.offset = (*block).ReadInt(16 + number * 12 + 8);
					result.push_back(tmp);

				}
				number++;
			}
			block2 = (*block).ReadInt(16 + number * 12);

		}
	}
}

//delete就直接重新读取重新建立就好了，纯调用，与类型无关
int index_Delete_record(Index& index_info, Table &table,
	Value& delete_value, Buffer & buffer1) {
	int ret_number;
	//Value find_delete = delete_value;
	vector<RecordAccess> delete_record;
	index_Find_RecordAccess(index_info, buffer1, delete_value, delete_record); //things to be deleted is in find_value.search_ressult
	ret_number = delete_record.size();
	if (delete_record.size() == 0) {
		//nothing to delete
		return ret_number;
	}
	vector<RecordAccess>::iterator delete_iter;
	for (delete_iter = delete_record.begin(); delete_iter != delete_record.end(); delete_iter++) {
		(*delete_iter).Erase(buffer1);
	}
	Drop_index(index_info, buffer1);
	Create_index(index_info, table, buffer1);
	return ret_number;
}

void index_Insert_record(Index& index_info, Table &table,
	RecordAccess new_record, Buffer & buffer1) {
	if (index_info.AttributeType == -1)
		Insert_int(index_info, table, new_record, buffer1);
	else if (index_info.AttributeType == 0) {
		Insert_float(index_info, table, new_record, buffer1);
	}
	else
		Insert_string(index_info, table, new_record, buffer1);
}

void Insert_float(Index& index_info, Table &table,
	RecordAccess new_record, Buffer & buffer1) {
	Record find_value1 = new_record.Read(buffer1);
	int int_value = find_value1.GetInt(index_info.AttributeOffset);
	float find_value = int_to_float(int_value);
	//
	int int_v;
	float float_v;
	//
	BlockID root = index_info.BlockNumber;
	vector <int> index_value;
	BlockID father = root;
	int isleaf;
	int children_number;
	Block * block;
	int number = 0;
	while (1) {
		block = buffer1.Retrieve(father);
		isleaf = (*block).ReadInt(8);
		if (isleaf) {
			break;
		}
		children_number = (*block).ReadInt(4);
		for (number = 0; number < children_number; number++) {
			int_v = (*block).ReadInt(16 + number * 8);
			float_v = int_to_float(int_v);
			if (float_v > find_value) {
				break;
			}
		}
		//judge
		if (number == 0) {
			father = (*block).ReadInt(16 + 4);
		}
		else {
			father = (*block).ReadInt(16 + number * 8 - 4);
		}
	}
	//找到相应的叶子
	int capacity = (*block).ReadInt(0);
	int size = (*block).ReadInt(4);
	int sibling_block = (*block).ReadInt(16 + size * 12);
	RecordAccess tmp;
	int num = 0;
	map<float, RecordAccess> records_new;

	if (capacity == size || find_value < int_to_float((*block).ReadInt(16))) {
		//rebuilt
		Drop_index(index_info, buffer1);
		Create_index(index_info, table, buffer1);
		return;
	}
	else {
		while (num < size) {
			tmp.bid = (*block).ReadInt(16 + num * 12 + 4);
			tmp.offset = (*block).ReadInt(16 + num * 12 + 8);
			records_new.insert(pair<float, RecordAccess>(int_to_float((*block).ReadInt(16 + num * 12)), tmp));
			num++;
		}

		records_new.insert(pair<float, RecordAccess>(find_value, new_record));
	}
	map< float, RecordAccess >::iterator new_iter = records_new.begin();
	//vector <Block*> block_nouse;
	/*Store_Leaf_Node(buffer1, block_nouse, size + 1, capacity, new_iter);*/
	int iter_number = 0;
	//block_ptr2.push_back(newleaf);//存储这个节点的指针
	(*block).WriteInt(0 * 4, capacity);
	(*block).WriteInt(1 * 4, size + 1);

	(*block).WriteInt(2 * 4, 1);
	(*block).WriteInt(3 * 4, 0);//父节点

	//循环写入
	while (iter_number < size + 1) {
		(*block).WriteInt((iter_number) * 12 + 16, float_to_int(new_iter->first));
		(*block).WriteInt((iter_number) * 12 + 4 + 16, new_iter->second.bid);
		(*block).WriteInt((iter_number) * 12 + 8 + 16, new_iter->second.offset);
		new_iter++;
		iter_number++;
	}

	(*block).WriteInt(16 + iter_number * 12, sibling_block);
	(*block).DirtyBit = 1;
	//finished
}

void Insert_int(Index& index_info, Table &table,
	RecordAccess new_record, Buffer & buffer1) {
	Record find_value1 = new_record.Read(buffer1);
	int find_value = find_value1.GetInt(index_info.AttributeOffset);
	BlockID root = index_info.BlockNumber;
	vector <int> index_value;
	BlockID father = root;
	int isleaf;
	int children_number;
	Block * block;
	int number = 0;
	while (1) {
		block = buffer1.Retrieve(father);
		isleaf = (*block).ReadInt(8);
		if (isleaf) {
			break;
		}
		children_number = (*block).ReadInt(4);
		for (number = 0; number < children_number; number++) {
			if ((*block).ReadInt(16 + number * 8) > find_value) {
				break;
			}
		}
		//judge
		if (number == 0) {
			father = (*block).ReadInt(16 + 4);
		}
		else {
			father = (*block).ReadInt(16 + number * 8 - 4);
		}
	}
	//找到相应的叶子
	int capacity = (*block).ReadInt(0);
	int size = (*block).ReadInt(4);
	int sibling_block = (*block).ReadInt(16 + size * 12);
	RecordAccess tmp;
	int num = 0;
	map<int, RecordAccess> records_new;
	if (capacity == size || find_value < (*block).ReadInt(16)) {
		//rebuilt
		Drop_index(index_info, buffer1);
		Create_index(index_info, table, buffer1);
		return;
	}
	else {
		while (num < size) {
			tmp.bid = (*block).ReadInt(16 + num * 12 + 4);
			tmp.offset = (*block).ReadInt(16 + num * 12 + 8);
			records_new.insert(pair<int, RecordAccess>((*block).ReadInt(16 + num * 12), tmp));
			num++;
		}

		records_new.insert(pair<int, RecordAccess>(find_value, new_record));
	}
	map< int, RecordAccess >::iterator new_iter = records_new.begin();
	//vector <Block*> block_nouse;
	/*Store_Leaf_Node(buffer1, block_nouse, size + 1, capacity, new_iter);*/
	int iter_number = 0;
	//block_ptr2.push_back(newleaf);//存储这个节点的指针
	(*block).WriteInt(0 * 4, capacity);
	(*block).WriteInt(1 * 4, size + 1);

	(*block).WriteInt(2 * 4, 1);
	(*block).WriteInt(3 * 4, 0);//父节点

	//循环写入
	while (iter_number < size + 1) {
		(*block).WriteInt((iter_number) * 12 + 16, new_iter->first);
		(*block).WriteInt((iter_number) * 12 + 4 + 16, new_iter->second.bid);
		(*block).WriteInt((iter_number) * 12 + 8 + 16, new_iter->second.offset);
		new_iter++;
		iter_number++;
	}

	(*block).WriteInt(16 + iter_number * 12, sibling_block);
	(*block).DirtyBit = 1;
	//finished
}
void Insert_string(Index& index_info, Table &table,
	RecordAccess new_record, Buffer & buffer1) {
	Record find_value1 = new_record.Read(buffer1);
	string find_value = find_value1.GetString(index_info.AttributeOffset);
	BlockID root = index_info.BlockNumber;
	vector <int> index_value;
	BlockID father = root;
	int isleaf;
	int children_number;
	Block * block;
	int number = 0;
	string value_str;
	int value[64]; int i = 0;
	while (1) {
		block = buffer1.Retrieve(father);
		isleaf = (*block).ReadInt(8);
		if (isleaf) {
			break;
		}
		children_number = (*block).ReadInt(4);
		for (number = 0; number < children_number; number++) {
			//
			for (i = 0; i < 64; i++) {
				value[i] = (*block).ReadInt(16 + number * 260 + 4 * i);
			}
			value_str = int_to_string(value);
			//
			if (value_str > find_value) {
				break;
			}
		}
		//judge
		if (number == 0) {
			father = (*block).ReadInt(16 + 256);
		}
		else {
			father = (*block).ReadInt(16 + number * 260 - 4);
		}
	}
	//找到相应的叶子
	int capacity = (*block).ReadInt(0);
	int size = (*block).ReadInt(4);
	int sibling_block = (*block).ReadInt(16 + size * 264);
	RecordAccess tmp;
	int num = 0;
	map<string, RecordAccess> records_new;
	for (i = 0; i < 64; i++) {
		value[i] = (*block).ReadInt(16 + 4 * i);
	}
	value_str = int_to_string(value);
	if (capacity == size || find_value < value_str){
		//rebuilt
		Drop_index(index_info, buffer1);
		Create_index(index_info, table, buffer1);
		return;
	}
	else {
		while (num < size) {
			tmp.bid = (*block).ReadInt(16 + num * 264 + 256);
			tmp.offset = (*block).ReadInt(16 + num * 264 + 260);
			//
			for (i = 0; i < 64; i++) {
				value[i] = (*block).ReadInt(16 + num * 264 + 4 * i);
			}
			value_str = int_to_string(value);
			//
			records_new.insert(pair<string, RecordAccess>(value_str, tmp));
			num++;
		}

		records_new.insert(pair<string, RecordAccess>(find_value, new_record));
	}
	map< string, RecordAccess >::iterator new_iter = records_new.begin();

	int iter_number = 0;
	int write[64];
	(*block).WriteInt(0 * 4, capacity);
	(*block).WriteInt(1 * 4, size + 1);

	(*block).WriteInt(2 * 4, 1);
	(*block).WriteInt(3 * 4, 0);//父节点

	//循环写入
	while (iter_number < size + 1) {
		//
		string_to_int(new_iter->first, write);
		for (i = 0; i < 64; i++) {
			(*block).WriteInt((iter_number) * 264 + 4 * i + 16, write[i]);
		}
		//
		(*block).WriteInt((iter_number) * 264 + 256 + 16, new_iter->second.bid);
		(*block).WriteInt((iter_number) * 264 + 260 + 16, new_iter->second.offset);
		new_iter++;
		iter_number++;
	}

	(*block).WriteInt(16 + iter_number * 264, sibling_block);
	(*block).DirtyBit = 1;
	//finished
}

