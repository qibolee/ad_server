adģ�飬�����津��

��ȡ���ε���ʣ����ݱ��صĹ���ļ�����ȡ���������������Ĺ��(��������������ֱ�ӽض�)�����ظ����ι����Ϣ

�������ݸ�ʽ: 
{
    adnum: int
    bidwords: [str, str, ...]
}

���صĹ�����ݸ�ʽ:
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

��������ļ���ʽ: '\t': winfoid, bidword, bid, q, title, desc1, desc2, targeturl, showurl
winfoid: uint64_t
bidword: str
bid: int
q: int
title: str
desc1: str
desc2: str
targeturl: str
showurl: str


gsp: ������ۼƷ�
����: q * bid
�Ƽ�: price1 = q2 * bid2 / q1 (q1 != 0) or bid1 (q1 == 0)
����: q2 * bid2 <= q1 * bid1 =>
q2 * bid2 / q1 <= bid1 =>
price1 <= bid1


