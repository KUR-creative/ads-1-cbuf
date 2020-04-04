import subprocess as sp
import sys

import yaml
import matplotlib.pyplot as plt
import funcy as F
from tqdm import tqdm


def expr_result(num_data):
    ''' experiment result in milli second '''
    return yaml.safe_load(
        sp.check_output(
            ['./main', str(num_data)],
            universal_newlines=True))

#min_n = 3 if len(sys.argv) != 3 else int(sys.argv[1])
#max_n = 10 if len(sys.argv) != 3 else int(sys.argv[2])

#num_expr = 3 if len(sys.argv) != 2 else int(sys.argv[1])
num_expr = 10 if len(sys.argv) != 2 else int(sys.argv[1])
#block = 2**29 // num_expr
block = 2**10 // num_expr
exprs = list(tqdm(
    map(expr_result, [n * block for n in range(1,num_expr)]),
    total = num_expr
))

with open('result_%d.yml' % (num_expr,), 'w') as f:
    yaml.dump(F.join_with(list, exprs), f)
with open('result_%d.yml' % (num_expr,)) as f:
    result_dic = yaml.safe_load(f)

y_keys = F.lremove('data.num', result_dic.keys())
#print(F.lmap(result_dic, y_keys))

import numpy as np
xs = result_dic['data.num']
gradient_dic = {}
for key in y_keys:
    plt.plot(xs, result_dic[key], label=key,
             marker='x' if 'q' in key else 'o',
             linestyle='--' if 'q' in key else '-',)
    gradient_dic[key] = np.polyfit(xs, result_dic[key], 1)[0]
    
print(gradient_dic)
with open('result_%d_gradient.yml' % (num_expr,), 'w') as f:
    yaml.dump(gradient_dic, f)
plt.xlabel('number of items')
plt.ylabel('milli seconds')
plt.legend()
plt.show()
