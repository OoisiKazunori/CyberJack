#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/KazBufferHelper.h"

/// <summary>
/// 使用していないハンドルを返し、指定のハンドルを削除できるクラス
/// </summary>
class HandleMaker
{
public:
	HandleMaker();

	/// <summary>
	///返り値でハンドルを返してくれます
	/// </summary>
	/// <returns>使用していないハンドル</returns>
	short GetHandle();

	/// <summary>
	/// 指定のハンドルを削除します
	/// </summary>
	/// <param name="HANDLE">指定のハンドル</param>
	void DeleteHandle(short HANDLE);


	bool CheckHandleWasDeleteOrNot(const short &HANDLE);
	bool CheckHandleWasUsedOrNot(const short &HANDLE);

	void SetHandleSize(const BufferMemorySize &SIZE);


	short CaluNowHandle(const short &HANDLE);

	short minSize, maxSize;
private:
	short handle;
	short setHandle;
	std::vector<short> deleteHandleNumber;

	/// <summary>
	/// ハンドルが過去に削除されていたかどうか検索します
	/// </summary>
	/// <param name="HANDLE">ハンドル</param>
	/// <returns>true...削除されていました,false...削除されてませんでした</returns>
	bool IsItDeleted(short HANDLE);
};