import pandas as pd
import numpy as np
import os
from matplotlib import pyplot as plt

def generate_chart(inv_data, swp_data, n_data):
    plt.scatter(n_data, inv_data, lebel='inv')
    z = np.polyfit(n_data, inv_data, 3)
    p = np.poly1d(z)
    plt.plot(n_data,p(n_data),"b--")
    
    plt.scatter(n_data, swp_data, label='swp')
    z = np.polyfit(n_data, swp_data, 3)
    p = np.poly1d(z)
    plt.plot(n_data,p(n_data),"r--")

    plt.xlabel('vertices number')    
    plt.ylabel('average error [%]')
    plt.legend()
    plt.savefig(f'plots/inv_vs_swp.png', bbox_inches='tight')
    plt.close()


def get_average(filename):
    data = pd.read_csv(filename, sep=';', skiprows=1)

    filtered_data = data[['error', 'mutate']]

    inv = np.where(filtered_data['mutate'] == 'INV')
    swp = np.where(filtered_data['mutate'] == 'SWP')

    inv_error = filtered_data.loc[inv]['error']
    swp_error = filtered_data.loc[swp]['error']

    inv_average_error = np.average(inv_error)
    swp_average_error = np.average(swp_error)

    vertices = (int)(filename.split('.')[0])

    return inv_average_error, swp_average_error, vertices


def filter(string, substr):
    return [str for str in string if
             any(sub in str for sub in substr)]


def main():
    path = "."
    dir_list = os.listdir(path)
    files = [name for name in dir_list if name.split('.')[-1] == 'csv'] 

    inv_data = [] 
    swp_data = [] 
    n_data = []

    for file in files:
        inv, swp, n = get_average(file)
        inv_data.append(inv)
        swp_data.append(swp)
        n_data.append(n)

    generate_chart(inv_data, swp_data, n_data)


if __name__ == '__main__':
    main()