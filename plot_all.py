#Arno Annys
#arno.annys@student.uantwerpen.be
#28 mei 2021
#functies voor plots en aanroepen in overeenstemming main uit c++

import matplotlib.pyplot as plt
import numpy as np

#counters die overeenkommen met nummers uit elke c++ simu
count1 = 0 #deze met 4 incrementern voor elke simu
count2 = 0 #deze met 1 incrementeren voor elke simu

def plot_3():
	p = np.genfromtxt("py-data" + str(count1)+ ".csv" , delimiter = "," )
	u = np.genfromtxt("py-data" + str(count1+1) + ".csv" , delimiter = "," )
	v = np.genfromtxt("py-data" + str(count1+2) + ".csv" , delimiter = "," )
	vort =  np.genfromtxt("py-data" + str(count1+3) + ".csv" , delimiter = "," )
	x = np.genfromtxt("x" + str(count2) + ".csv" , delimiter = "," )
	y = np.genfromtxt("y" + str(count2) + ".csv" , delimiter = "," )
	p = np.where(p ==  0.000042 , np.nan , p) 
	x = np.multiply(x,2./(len(x)-1)) 
	y  = np.multiply(y,2./(len(x)-1))
	
	
	plt.figure()
	plt.contourf(x,y,np.flipud(p), 100,cmap = 'jet' )
	plt.colorbar()
	plt.quiver(x ,y , np.flipud(u ),-np.flipud(v))
	plt.title('druk als kleurenmap met snelheid als pijlen')
	plt.xlabel('x')
	plt.ylabel('y')
	plt.xlim(0,2)
	plt.ylim(0,2)
	plt.savefig('p_im' + str(count2) + '.png')
	
	plt.figure()
	plt.contourf(x,y,np.flipud(np.sqrt(u*u + v*v) ), 100,cmap = 'jet')
	plt.colorbar()
	plt.quiver(x ,y , np.flipud(u) , -np.flipud(v))
	plt.title('norm snelheid als kleurenmap met snelheid als pijlen')
	plt.xlabel('x')
	plt.ylabel('y')
	plt.xlim(0,2)
	plt.ylim(0,2)
	plt.savefig('vel_im' + str(count2) + '.png')
	
	plt.figure()
	plt.contourf(x,y,np.flipud(vort) , 100,cmap = 'jet' )
	plt.colorbar()
	plt.streamplot(x[1,:] ,y[:,1] ,np.flipud(u), -np.flipud(v),2)
	plt.title('vorticiteit als kleurenmap met snelheid als stroomlijnen')
	plt.xlabel('x')
	plt.xlim(0,2)
	plt.ylim(0,2)
	plt.ylabel('y')
	plt.savefig('vort_im' + str(count2) + '.png')
	
def u_profile(A):
	u = np.genfromtxt("py-data" + str(count1+1) + ".csv" , delimiter = "," )
	x = np.genfromtxt("x" + str(count2) + ".csv" , delimiter = "," )
	x = np.multiply(x,2./(len(x)-1))
	plt.figure()
	plt.plot( u[:,A],np.flipud(x[1,0:len(x)] ))
	plt.title('u profiel')
	plt.xlabel('u')
	plt.ylabel('y')
	plt.savefig('u_im' +str( count2)+'-'+str(A)+ '.png')

def v_profile(A):
	v = np.genfromtxt("py-data" + str(count1+2) + ".csv" , delimiter = "," )
	y = np.genfromtxt("y" + str(count2) + ".csv" , delimiter = "," )
	y  = np.multiply(y,2./(len(y)-1))
	plt.figure()
	plt.plot( y[0:len(y),1],np.flipud(v[A,0:len(y)] ))
	plt.title('v profiel')
	plt.xlabel('x')
	plt.ylabel('v')
	plt.savefig('v_im' + str(count2)+'-' +str(A) + '.png')


########aanroepen plots###########
########gedreven caviteit##########

plot_3()
count1 = count1 + 4
count2 = count2 + 1
plot_3()
count1 = count1 + 4
count2 = count2 + 1
plot_3()
count1 = count1 + 4
count2 = count2 + 1
plot_3()
u_profile(20)
count1 = count1 + 4
count2 = count2 + 1
########BUIS########
plot_3()
u_profile(20)
count1 = count1 + 4
count2 = count2 + 1
plot_3()
u_profile(20)
count1 = count1 + 4
count2 = count2 + 1
plot_3()
u_profile(20)
count1 = count1 + 4
count2 = count2 + 1

########BUIS met obstakel########
plot_3()
u_profile(30)
v_profile(30)
count1 = count1 + 4
count2 = count2 + 1
plot_3()
u_profile(45)
v_profile(10)
count1 = count1 + 4
count2 = count2 + 1
plot_3()
u_profile(12)
v_profile(35)
count1 = count1 + 4
count2 = count2 + 1
plot_3()
u_profile(20)
v_profile(26)
count1 = count1 + 4
count2 = count2 + 1

