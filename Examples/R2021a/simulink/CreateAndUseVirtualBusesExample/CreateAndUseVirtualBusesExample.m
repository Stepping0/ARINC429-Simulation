%% Group Signal Lines Within a Component
% You can group signals into a virtual bus within a component by using Bus
% Creator blocks.

% Copyright 2019 The MathWorks, Inc.

%%
% Open the example model, which contains three blocks.

open_system('VirtualBusCreationModel');

%%
% To create a bus that contains the outputs from multiple blocks, drag to
% select the blocks. For this example, select the Chirp Signal and Sine
% Wave blocks. In the action bar that appears, click *Create Bus*.
%
% <<../VirtualBusCreationActionBar.png>>
%
% Simulink adds a Bus Creator block and connects the inputs to that block.
% The output of the Bus Creator block is a virtual bus.
%
% <<../VirtualBusCreation1.png>>

%%
% To make identifying the elements of the bus easier, label the inputs to
% the Bus Creator block. Label the output of the Chirp Signal block by
% double-clicking the line and entering |Chirp|. Similarly, label the
% output of the Sine Wave block |Sine|.

%% 
% To create a second bus that contains the first bus and the output of the
% Step block, drag to select the Bus Creator and Step blocks. In the action
% bar that appears, click *Create Bus*. Since the |Sine| and |Chirp|
% signals are elements of the input bus, Simulink creates the same bus
% regardless of whether your selection includes the Sine Wave and Chirp
% Signal blocks.
%
% Simulink adds another Bus Creator block and connects the inputs to that
% block. The output of the Bus Creator block is a virtual bus that contains
% a nested bus.
%
% <<../VirtualBusCreation2.png>>
%
% You can nest buses to any depth. If one of the inputs to a Bus Creator
% block is a bus, then its output is a bus hierarchy that contains at least
% one nested bus.

%% 
% Label the output of the Step block |Step| and the output of the first Bus
% Creator block |NestedBus|. Optionally, arrange the blocks to improve the
% readability of the model. 

%% 
% Connect the output of the second Bus Creator block to a Scope block and
% label the output |TopBus|. To visually identify the buses, compile the
% model by clicking *Update Model* or *Run* on the *Modeling* tab of the
% Simulink Toolstrip. Compiling the model updates the line styles.
%
% <<../VirtualBusCreationFinal.png>>