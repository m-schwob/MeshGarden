


# original solution from https://stackoverflow.com/questions/579310/formatting-long-numbers-as-strings-in-python


def human_format_orig(num):
    num = float('{:.3g}'.format(num))
    magnitude = 0
    while abs(num) >= 1000:
        magnitude += 1
        num /= 1000.0
    return '{}{}'.format('{:f}'.format(num).rstrip('0').rstrip('.'), ['', 'K', 'M', 'B', 'T'][magnitude])


# print number in a readable format.
# default is up to 3 decimal digits and can be changed
# works on numbers in the range of 1e-15 to 1e 1e15 include negatives numbers
# can force the number to a specific magnitude unit
def human_format(num:float, force=None, ndigits=3):
    perfixes = ('p', 'n', 'u', 'm', '', 'K', 'M', 'G', 'T')
    one_index = perfixes.index('')
    if force:
        if force in perfixes:
            index = perfixes.index(force)
            magnitude = 3*(index - one_index)
            num = num/(10**magnitude)
        else:
            raise ValueError('force value not supported.')
    else:
        div_sum = 0
        if(abs(num) >= 1000):
            while abs(num) >= 1000:
                div_sum += 1
                num /= 1000
        else:
            while abs(num) <= 1:
                div_sum -= 1
                num *= 1000
        temp = round(num, ndigits) if ndigits else num
        if temp < 1000:
            num = temp 
        else:
            num = 1
            div_sum += 1
        index = one_index + div_sum
    return str(num).rstrip('0').rstrip('.') + perfixes[index]

# some tests
print(human_format(999)              ,' = '         , '999') 
print(human_format(999.999)          ,' = '         , '999.999') 
print(human_format(999.9999)         ,' = '         , '1K')  
print(human_format(999999)           ,' = '         , '999.999K')   
print(human_format(999499)           ,' = '         , '999.499K')   
print(human_format(9994)             ,' = '         , '9.994K')   
print(human_format(9900)             ,' = '         , '9.9K')   
print(human_format(6543165413)       ,' = '         , '6.543G')  
print(human_format(46780.9)          ,' = '         , '46.781K')  
print(human_format(0.001)            ,' = '         , '1m')   
print(human_format(0.000000999999)   ,' = '         , '999.999n')  
print(human_format(1.00394200)       ,' = '         , '1.004')   
print(human_format(0.0999)           ,' = '         , '99.9m')  
print(human_format(0.00000000999999) ,' = '         , '10n') 
print(human_format(0.0000000099995)  ,' = '         , '9.999n')  
print(human_format(0.000000009999)   ,' = '         , '9.999n') 
print(human_format(999999            ,ndigits=2)    ,' = '           , '1M')   
print(human_format(9994              ,force='')     ,' = '           , '9994K')   
print(human_format(6543165413        ,ndigits=5)    ,' = '           , '6.54317G')  
print(human_format(6543165413        ,ndigits=None) ,' = '           , '6.543165413G')  
print(human_format(7436313           ,ndigits=2)    ,' = '           , '7.44M')   
print(human_format(2344              ,ndigits=2)    ,' = '           , '2.34K')
print(human_format(34867123012.13    ,ndigits=2)    ,' = '           , '34.87G')   

