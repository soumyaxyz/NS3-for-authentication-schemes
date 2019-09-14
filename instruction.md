# Instructions 
##  The raw file
The attached file has **fifteen abstracts**  numbered from **n** to **n+15**. 
A slice from a file containing two abstracts are shown below :

    # 10
    ### http://arxiv.org/abs/cs/0411017v1
    ## Analysis of 802.11b MAC: A QoS, Fairness, and Performance Perspective
    
    -1 	Wireless LANs have achieved a tremendous amount of growth in recent years.
    -1 	Among various wireless LAN technologies, the IEEE 802.
    -1 	11b based wireless LAN technology can be cited as the most prominent technology today.
    -1 	Despite being widely deployed, 802.
    -1 	11b cannot be termed as a well matured technology.
    -1 	Although 802.
    -1 	11b is adequate for basic connectivity and packet switching, It is evident that there is ample scope for its improvement in areas like quality of service, fairness, performance, security, etc.
    -1 	In this survey report, we identify and argue that the Medium Access Controller for 802.
    -1 	11b networks is the prime area for these improvements.
    -1 	To enunciate our claims we highlight some of the quality of service, fairness, and performance issues related to 802.
    -1 	11b MAC.
    -1 	We also describe and analyze some of the current research aimed at addressing these issues.
    -1 	We then propose a novel scheme called the Intelligent Collision Avoidance, seeking to enhance the MAC to address some of the performance issues in 802.
    -1 	11b and similar networks.

    
    # 11
    ### http://arxiv.org/abs/cs/0411054v1
    ## J2EE Deployment: The JOnAS Case Study
    
    -1 	La specification J2EE (Java 2 platform Enterprise Edition) definit une architecture de serveur d'application Java.
    -1 	Jusqu'a J2EE 1.
    -1 	3, seuls les aspects de deploiement concernant le developpeur d'applications etaient adresses.
    -1 	Avec J2EE 1.
    -1 	4, les interfaces et les etapes de deploiement ont ete plus precisement specifiees dans la specification "J2EE Deployment".
    -1 	JOnAS (Java Open Application Server) est une plate-forme J2EE developpee au sein du consortium ObjectWeb.
    -1 	Les aspects deploiement sont en cours de developpement.
    -1 	Cet article decrit les concepts lies au deploiement dans J2EE, ainsi que les problematiques levees lors de leur mise en oeuvre pour JOnAS.
    -1 	Il n'a pas pour but de presenter un travail abouti, mais illustre le deploiement par un cas concret et ebauche une liste de besoins non encore satisfaits dans le domaine.
    -1 	-----   The J2EE (Java 2 platform Enterprise Edition) specification defines an architecture for Java Application Servers.
    -1 	Until J2EE 1.
    -1 	3, the deployment aspect was addressed from the developer point of view only.
    -1 	Since J2EE 1.
    -1 	4, deployment APIs and steps have been more precisely specified within the "J2EE Deployment Specification".
    -1 	JOnAS (Java Open Application Server) is a J2EE platform implementation by ObjectWeb.
    -1 	The deployment aspects are under development.
    -1 	This article describes the J2EE Deployment concepts, and the issues raised when implementing deployment features within JOnAS.
    -1 	It does not provide a complete solution, but illustrates deployment through a concrete example and initiates a list of non fulfilled requirements.


## Step one

First, identify where lines were unnecessarily  divided and merge them back. The errors exists mostly in case where the **.** symblo  is used, but not a period.

The erronous line breaks from the previous sample are fixed below:

    # 10
    ### http://arxiv.org/abs/cs/0411017v1
    ## Analysis of 802.11b MAC: A QoS, Fairness, and Performance Perspective
    
    -1 	Wireless LANs have achieved a tremendous amount of growth in recent years.
~~    -1 	Among various wireless LAN technologies, the IEEE 802.
    -1 	11b based wireless LAN technology can be cited as the most prominent technology today.
    ~~
    -1 	Despite being widely deployed, 802.
    -1 	11b cannot be termed as a well matured technology.
    -1 	Although 802.
    -1 	11b is adequate for basic connectivity and packet switching, It is evident that there is ample scope for its improvement in areas like quality of service, fairness, performance, security, etc.
    -1 	In this survey report, we identify and argue that the Medium Access Controller for 802.
    -1 	11b networks is the prime area for these improvements.
    -1 	To enunciate our claims we highlight some of the quality of service, fairness, and performance issues related to 802.
    -1 	11b MAC.
    -1 	We also describe and analyze some of the current research aimed at addressing these issues.
    -1 	We then propose a novel scheme called the Intelligent Collision Avoidance, seeking to enhance the MAC to address some of the performance issues in 802.
    -1 	11b and similar networks.

    
    # 11
    ### http://arxiv.org/abs/cs/0411054v1
    ## J2EE Deployment: The JOnAS Case Study
    
    -1 	La specification J2EE (Java 2 platform Enterprise Edition) definit une architecture de serveur d'application Java.
    -1 	Jusqu'a J2EE 1.
    -1 	3, seuls les aspects de deploiement concernant le developpeur d'applications etaient adresses.
    -1 	Avec J2EE 1.
    -1 	4, les interfaces et les etapes de deploiement ont ete plus precisement specifiees dans la specification "J2EE Deployment".
    -1 	JOnAS (Java Open Application Server) est une plate-forme J2EE developpee au sein du consortium ObjectWeb.
    -1 	Les aspects deploiement sont en cours de developpement.
    -1 	Cet article decrit les concepts lies au deploiement dans J2EE, ainsi que les problematiques levees lors de leur mise en oeuvre pour JOnAS.
    -1 	Il n'a pas pour but de presenter un travail abouti, mais illustre le deploiement par un cas concret et ebauche une liste de besoins non encore satisfaits dans le domaine.
    -1 	-----   The J2EE (Java 2 platform Enterprise Edition) specification defines an architecture for Java Application Servers.
    -1 	Until J2EE 1.
    -1 	3, the deployment aspect was addressed from the developer point of view only.
    -1 	Since J2EE 1.
    -1 	4, deployment APIs and steps have been more precisely specified within the "J2EE Deployment Specification".
    -1 	JOnAS (Java Open Application Server) is a J2EE platform implementation by ObjectWeb.
    -1 	The deployment aspects are under development.
    -1 	This article describes the J2EE Deployment concepts, and the issues raised when implementing deployment features within JOnAS.
    -1 	It does not provide a complete solution, but illustrates deployment through a concrete example and initiates a list of non fulfilled requirements.  
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEwMDAyNTQ2OTEsLTIwMzQzODU2NTUsLT
c0NjE1MDUwMSwxOTg4ODg0MzQwXX0=
-->