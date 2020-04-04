import subprocess as sp
import sys

import yaml
import matplotlib.pyplot as plt
import funcy as F
from tqdm import tqdm


def expr_result(num_data):
    ''' experiments results in milli second '''
    return yaml.safe_load(
        sp.check_output(
            ['./main', str(num_data)],
            universal_newlines=True))

min_n = 3 if len(sys.argv) != 3 else int(sys.argv[1])
max_n = 10 if len(sys.argv) != 3 else int(sys.argv[2])
exprs = list(tqdm(
    map(expr_result, [2**n for n in range(min_n,max_n)]),
    total = max_n - min_n + 1
))

with open('result_%d_%d.yml' % (min_n, max_n), 'w') as f:
    yaml.dump(F.join_with(list, exprs), f)
with open('result_%d_%d.yml' % (min_n, max_n)) as f:
    result_dic = yaml.safe_load(f)

y_keys = F.lremove('data.num', result_dic.keys())
#print(F.lmap(result_dic, y_keys))

xs = result_dic['data.num']
for key in y_keys:
    plt.plot(xs, result_dic[key], label=key,
             marker='x' if 'q' in key else 'o',
             linestyle='--' if 'q' in key else '-',)
    
plt.xlabel('number of items')
plt.ylabel('milli seconds')
plt.legend()
plt.show()
