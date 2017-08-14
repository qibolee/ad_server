ad模块，负责广告触发

读取上游的买词，根据本地的广告文件，提取出所有满足条件的广告(超出数量限制则直接截断)，返回给上游广告信息

请求数据格式: 
{
    adnum: int
    bidwords: [str, str, ...]
}

返回的广告数据格式:
[
    {# ad1
        winfoid: str
        bidword: str
        bid: int
        q: int
		charge: int
        desc1: str
        desc2: str
        showurl: str
        targeturl: str
    }
    {# ad2
        ...
    }
	...
]

广告数据文件格式: '\t': winfoid, bidword, bid, q, title, desc1, desc2, targeturl, showurl
winfoid: uint64_t
bidword: str
bid: int
q: int
title: str
desc1: str
desc2: str
targeturl: str
showurl: str


gsp: 广义二价计费
排序: q * bid
计价: price1 = q2 * bid2 / q1 (q1 != 0) or bid1 (q1 == 0)
推论: q2 * bid2 <= q1 * bid1 =>
q2 * bid2 / q1 <= bid1 =>
price1 <= bid1


