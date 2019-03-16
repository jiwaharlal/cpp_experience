Task description:

https://www.hackerrank.com/challenges/reverse-shuffle-merge/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=greedy-algorithms&h_r=next-challenge&h_v=zen&h_r=next-challenge&h_v=zen

Algo:

1. "divide" string by characters - figure out character content of strings. let's name it str1
2. str = reverse initil string
3. char_set = set of chars in str1
4. start from str.begin()
5. look for minimum character, until all previous chars are subset of char_set
6. cut str untill this minimum char, substitute characters from char_set
7. if char_set empty or char_set.size() == str.size(), break
8. start from min_char_pos + 1, goto 5
