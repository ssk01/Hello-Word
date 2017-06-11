END = '$'

def make_trie(words):
    trie = {}
    for word in words:
        t = trie
        for c in word:
            if c not in t:
                t[c]={}
            t = t[c]
        t[END] = {}
    return trie


#tol = num of wrong letters that can be stand
def check_fuzzy(trie, word, path, tol=1):
    if tol < 0:
        return set()
    elif word == "":
        if END in trie:
            return {path}
        else:
            for c in set("abc"):
                rs = check_fuzzy(trie, c, path, tol-1)
                if  rs:
                    return rs
            return set()
        # return {path} if END in trie else set()
    else:
        # ps = set()
        # for k in trie:
        #     ps |= check_fuzzy(trie[k], word[1:], path+k,
        #      tol-1 if k!=word[0] else tol)
        # return ps
                
        ps =set()
        for k in trie:

            ps |= check_fuzzy(trie[k], word[1:], path+k, tol - 1 if k != word[0] else tol)
            # Ôö¼Ó×ÖÄ¸
            for c in set("abc"):
                b= check_fuzzy(trie, c+word, path, tol-1)
                if b:
                    print b
                    ps |=b
           # É¾¼õ×ÖÄ¸
            if len(word) > 0:
                b= check_fuzzy(trie, word[1:], path, tol-1)
                if b:
                    print b
                    ps |= b
            # ½»»»×ÖÄ¸
            if len(word) > 1:
                ps |= check_fuzzy(trie, word[1]+word[0]+word[2:], path, tol-1)
        return ps


words =['ac']
t= make_trie(words)
print check_fuzzy(t, 'aac',"", 1)

# print check_fuzzy(t, 'ab',"", 1)
# print check_fuzzy(t, 'bc',"", 1)
# print check_fuzzy(t, 'aabc',"", 1)
# print check_fuzzy(t, 'abcd',"", 1)
# print check_fuzzy(t, 'bac',"", 1)
# print check_fuzzy(t, 'acb',"", 1)

# print check_fuzzy(t,'helcd',"",tol =2)