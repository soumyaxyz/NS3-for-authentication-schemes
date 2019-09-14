# Instructions 
The instructions on how to label the data is detailed below.  If doubt presists, feel free to mail me for clarifications.
###  The structure of the provided file
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
     
    # 12
    ### http://arxiv.org/abs/cs/0411058v1
    ## Deployment in dynamic environments
    
    -1 	Information and communication technologies are moving towards a new stage where applications will be dynamically deployed, uninstalled, updated and (re)configured.
    -1 	Several approaches have been followed with the goal of creating a fully automated and context-aware deployment system.
    -1 	Ideally, this system should be capable of handling the dynamics of this new situation, without losing sight of other factors, such as performance, security, availability or scalability.
    -1 	We will take some of the technologies that follow the principles of Service Oriented Architectures, SOA, as a paradigm of dynamic environments.
    -1 	SOA promote the breaking down of applications into sets of loosely coupled elements, called services.
    -1 	Services can be dynamically bound, deployed, reconfigured, uninstalled and updated.
    -1 	First of all, we will try to offer a broad view on the specific deployment issues that arise in these environments.
    -1 	Later on, we will present our approach to the problem.
    -1 	One of the essential points that has to be tackled to develop an automated deployment engine will be to have enough information to carry out tasks without human intervention.
    -1 	In the article we will focus on the format and contents of deployment descriptors.
    -1 	Additionally, we will go into the details of the deployment framework for OSGi enabled gateways that has been developed by our research group.
    -1 	Finally we will give some concluding remarks and some ideas for future work



### Initial step

First, identify where lines were unnecessarily divided and **merge them back after removing the label from the secondary lines **. The errors exist mostly in case where the **.** symbol is used, but not as a period.

The erroneous line breaks from the previous sample are fixed below:

    # 10
    ### http://arxiv.org/abs/cs/0411017v1
    ## Analysis of 802.11b MAC: A QoS, Fairness, and Performance Perspective
    
    -1 	Wireless LANs have achieved a tremendous amount of growth in recent years.
    //-1 	Among various wireless LAN technologies, the IEEE 802.
    //-1 	11b based wireless LAN technology can be cited as the most prominent technology today.
    -1 	Among various wireless LAN technologies, the IEEE 802.11b based wireless LAN technology can be cited as the most prominent technology today.    
    //-1 	Despite being widely deployed, 802.
    //-1 	11b cannot be termed as a well matured technology.
    -1 	Despite being widely deployed, 802.11b cannot be termed as a well matured technology.
    //-1 	Although 802.
    //-1 	11b is adequate for basic connectivity and packet switching, It is evident that there is ample scope for its improvement in areas like quality of service, fairness, performance, security, etc.
     -1 	Although 802.11b is adequate for basic connectivity and packet switching, It is evident that there is ample scope for its improvement in areas like quality of service, fairness, performance, security, etc.
    //-1 	In this survey report, we identify and argue that the Medium Access Controller for 802.
    //-1 	11b networks is the prime area for these improvements.
    -1 	In this survey report, we identify and argue that the Medium Access Controller for 802.11b networks is the prime area for these improvements.
    //-1 	To enunciate our claims we highlight some of the quality of service, fairness, and performance issues related to 802.
    //-1 	11b MAC.
    -1 	To enunciate our claims we highlight some of the quality of service, fairness, and performance issues related to 802.11b MAC.     
    -1 	We also describe and analyze some of the current research aimed at addressing these issues.
    //-1 	We then propose a novel scheme called the Intelligent Collision Avoidance, seeking to enhance the MAC to address some of the performance issues in 802.
    //-1 	11b and similar networks.
    //-1 	We then propose a novel scheme called the Intelligent Collision Avoidance, seeking to enhance the MAC to address some of the performance issues in 802.
    //-1 	11b and similar networks.
    -1 	We then propose a novel scheme called the Intelligent Collision Avoidance, seeking to enhance the MAC to address some of the performance issues in 802.11b and similar networks.



    
    # 11
    ### http://arxiv.org/abs/cs/0411054v1
    ## J2EE Deployment: The JOnAS Case Study
    
    -1 	La specification J2EE (Java 2 platform Enterprise Edition) definit une architecture de serveur d'application Java.
    //-1 	Jusqu'a J2EE 1.
    //-1 	3, seuls les aspects de deploiement concernant le developpeur d'applications etaient adresses.
    -1 	Jusqu'a J2EE 1.3, seuls les aspects de deploiement concernant le developpeur d'applications etaient adresses.
    //-1 	Avec J2EE 1.
    //-1 	4, les interfaces et les etapes de deploiement ont ete plus precisement specifiees dans la specification "J2EE Deployment".
    -1 	Avec J2EE 1.4, les interfaces et les etapes de deploiement ont ete plus precisement specifiees dans la specification "J2EE Deployment".
    -1 	JOnAS (Java Open Application Server) est une plate-forme J2EE developpee au sein du consortium ObjectWeb.
    -1 	Les aspects deploiement sont en cours de developpement.
    -1 	Cet article decrit les concepts lies au deploiement dans J2EE, ainsi que les problematiques levees lors de leur mise en oeuvre pour JOnAS.
    -1 	Il n'a pas pour but de presenter un travail abouti, mais illustre le deploiement par un cas concret et ebauche une liste de besoins non encore satisfaits dans le domaine.
    //-1 	-----   The J2EE (Java 2 platform Enterprise Edition) specification defines an architecture for Java Application Servers.
    -1 	The J2EE (Java 2 platform Enterprise Edition) specification defines an architecture for Java Application Servers.
    //-1 	Until J2EE 1.
    //-1 	3, the deployment aspect was addressed from the developer point of view only.
    -1 	Until J2EE 1.3, the deployment aspect was addressed from the developer point of view only.
    //-1 	Since J2EE 1.
    //-1 	4, deployment APIs and steps have been more precisely specified within the "J2EE Deployment Specification".
    -1 	Since J2EE 1.4, deployment APIs and steps have been more precisely specified within the "J2EE Deployment Specification".
    -1 	JOnAS (Java Open Application Server) is a J2EE platform implementation by ObjectWeb.
    -1 	The deployment aspects are under development.
    -1 	This article describes the J2EE Deployment concepts, and the issues raised when implementing deployment features within JOnAS.
    -1 	It does not provide a complete solution, but illustrates deployment through a concrete example and initiates a list of non fulfilled requirements.
       
       
    # 12
    ### http://arxiv.org/abs/cs/0411058v1
    ## Deployment in dynamic environments
    
    -1 	Information and communication technologies are moving towards a new stage where applications will be dynamically deployed, uninstalled, updated and (re)configured.
    -1 	Several approaches have been followed with the goal of creating a fully automated and context-aware deployment system.
    -1 	Ideally, this system should be capable of handling the dynamics of this new situation, without losing sight of other factors, such as performance, security, availability or scalability.
    -1 	We will take some of the technologies that follow the principles of Service Oriented Architectures, SOA, as a paradigm of dynamic environments.
    -1 	SOA promote the breaking down of applications into sets of loosely coupled elements, called services.
    -1 	Services can be dynamically bound, deployed, reconfigured, uninstalled and updated.
    -1 	First of all, we will try to offer a broad view on the specific deployment issues that arise in these environments.
    -1 	Later on, we will present our approach to the problem.
    -1 	One of the essential points that has to be tackled to develop an automated deployment engine will be to have enough information to carry out tasks without human intervention.
    -1 	In the article we will focus on the format and contents of deployment descriptors.
    -1 	Additionally, we will go into the details of the deployment framework for OSGi enabled gateways that has been developed by our research group.
    -1 	Finally we will give some concluding remarks and some ideas for future work  


## Final step




Each sentences is initialy labeled as **-1** or undecided. Try to correctly label the sentences of the abstract  per the labels described below: 
Label|Meaning
-:|:-
-1| UNDECIDED|
0|BACKGROUND or OBJECTIVE
1|METHODS
2|RESULTS+CONCLUSIONS
If several lines in an abstract are hard to classfy, leave the entire abstract undecided.
**Do not label if in doubt**. Make sure the labels you put are accurate.
Attempt to label **at least  10 out of the 15** abstracts.

The abstracts samples from before are labeled. Since several lines of the second abstract is in French, the abstract cannot be labeled properly and is left undecided. 


    # 10
    ### http://arxiv.org/abs/cs/0411017v1
    ## Analysis of 802.11b MAC: A QoS, Fairness, and Performance Perspective
    
    0 	Wireless LANs have achieved a tremendous amount of growth in recent years.
    0 	Among various wireless LAN technologies, the IEEE 802.11b based wireless LAN technology can be cited as the most prominent technology today. 
    0 	Despite being widely deployed, 802.11b cannot be termed as a well matured technology.
    0 	Although 802.11b is adequate for basic connectivity and packet switching, It is evident that there is ample scope for its improvement in areas like quality of service, fairness, performance, security, etc.
    1 	In this survey report, we identify and argue that the Medium Access Controller for 802.11b networks is the prime area for these improvements.
    1 	To enunciate our claims we highlight some of the quality of service, fairness, and performance issues related to 802.11b MAC.     
    1 	We also describe and analyze some of the current research aimed at addressing these issues.
    1 	We then propose a novel scheme called the Intelligent Collision Avoidance, seeking to enhance the MAC to address some of the performance issues in 802.11b and similar networks.


    
    # 11
    ### http://arxiv.org/abs/cs/0411054v1
    ## J2EE Deployment: The JOnAS Case Study
    
    -1 	La specification J2EE (Java 2 platform Enterprise Edition) definit une architecture de serveur d'application Java.
    -1 	Jusqu'a J2EE 1.3, seuls les aspects de deploiement concernant le developpeur d'applications etaient adresses.
    -1 	Avec J2EE 1.4, les interfaces et les etapes de deploiement ont ete plus precisement specifiees dans la specification "J2EE Deployment".
    -1 	JOnAS (Java Open Application Server) est une plate-forme J2EE developpee au sein du consortium ObjectWeb.
    -1 	Les aspects deploiement sont en cours de developpement.
    -1 	Cet article decrit les concepts lies au deploiement dans J2EE, ainsi que les problematiques levees lors de leur mise en oeuvre pour JOnAS.
    -1 	Il n'a pas pour but de presenter un travail abouti, mais illustre le deploiement par un cas concret et ebauche une liste de besoins non encore satisfaits dans le domaine.
    -1 	The J2EE (Java 2 platform Enterprise Edition) specification defines an architecture for Java Application Servers.
    -1 	Until J2EE 1.3, the deployment aspect was addressed from the developer point of view only.
    -1 	Since J2EE 1.4, deployment APIs and steps have been more precisely specified within the "J2EE Deployment Specification".
    -1 	JOnAS (Java Open Application Server) is a J2EE platform implementation by ObjectWeb.
    -1 	The deployment aspects are under development.
    -1 	This article describes the J2EE Deployment concepts, and the issues raised when implementing deployment features within JOnAS.
    -1 	It does not provide a complete solution, but illustrates deployment through a concrete example and initiates a list of non fulfilled requirements.  
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTI0MTg0MjA4MSwtNTk0NDMwMTg1LC0yMD
M0Mzg1NjU1LC03NDYxNTA1MDEsMTk4ODg4NDM0MF19
-->