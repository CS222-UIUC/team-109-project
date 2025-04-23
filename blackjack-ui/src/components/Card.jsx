import React from 'react';
import { cardImageUrl } from '../utils/cardImages';

const Card = ({ card, hidden }) => {
    const src = hidden
        ? '/cards/astronaut.png'
        : cardImageUrl(card);

    return (
        <img
            src={src}
            alt={hidden ? 'Hidden card' : `${card.rank} of ${card.suit}`}
            width={175}
            height={200}
            className="object-cover rounded mx-1"
        />
    );
};


export default Card;